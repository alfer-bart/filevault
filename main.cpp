#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <chrono>
#include <future>

#include "aes.h"
#include "filevault.h"

using namespace std;

void EncryptAsync(void (*action)(UCHAR*, const UCHAR*, const int), AsyncData& ad) {
    ad.in_file.read((char*)ad.block, ASYNC_SIZE);
    for (int t = 0; t < THREAD_COUNT; t++) {
        ad.futures[t] = std::async(std::launch::async, action, ad.block + t * BLOCK_SIZE, ad.key, BLOCK_SIZE);
    }
    for (int t = 0; t < THREAD_COUNT; t++) {
        ad.futures[t].get();
    }
    ad.out_file.write((const char*)ad.block, ASYNC_SIZE);
}

void EncryptTrail(AsyncData& ad, int trail) {
    ad.in_file.read((char*)ad.block, trail);
    aes::AddPadding(ad.block, trail);
    aes::Encrypt(ad.block, ad.key, trail);
    ad.out_file.write((const char*)ad.block, trail);
}

void DecryptTrail(AsyncData& ad, int trail) {
    ad.in_file.read((char*)ad.block, trail);
    aes::Decrypt(ad.block, ad.key, trail);
    aes::RemovePadding(ad.block, trail);
    ad.out_file.write((const char*)ad.block, trail);
}

void ClearKey(char* key) {
    for (int i = 0; i < 16; i++)
        key[i] = 0;
}

int main() {
    char key[16];
    int trail;
    int mode = NONE;
    std::string cmd_line;
    std::streampos file_size;
    AsyncData data;

    std::cout << "FileVault 1.0\n\n";
    std::cout << "To store a file, type lock <file path> <password> <output file path>\n";
    std::cout << "To retrieve a file, type unlock instead of lock\n\n";
    std::cout << "Example 1: lock D:\\mypic.jpg 1234 D:\\jpg.locked\n";
    std::cout << "Example 2: unlock D:\\jpg.locked 1234 D:\\unlocked.jpg\n";
    std::cout << "File path must not contain spaces!\n";

    while (1) {
        ClearKey(key);
        std::getline(cin, cmd_line);
        std::istringstream iss(cmd_line);
        std::vector<std::string> input_data{
            std::istream_iterator<std::string>(iss), {}
        };

        if (input_data.size() < 4) {
            std::cout << "Error: input must contain 4 arguments\n";
            continue;
        }

        if (input_data[0] == "lock") {
            mode = LOCK;
        }
        else if (input_data[0] == "unlock") {
            mode = UNLOCK;
        }
        else {
            std::cout << "Unknown command: " << input_data[0] << endl;
            continue;
        }

        data.in_file.open(input_data[1], ios::ate | ios::binary);

        if (data.in_file.fail()) {
            std::cout << "Error: file is missing\n";
            continue;
        }

        // Get file size and go to the beginning of the file
        file_size = data.in_file.tellg();
        trail = file_size % ASYNC_SIZE;
        data.in_file.seekg(0, ios::beg);

        // Get key
        for (int i = 0; i < 16; i++) {
            if (input_data[2][i])
                key[i] = input_data[2][i];
            else
                break;
        }

        // Create output file
        data.out_file.open(input_data[3], ios::trunc | ios::binary);

        // Benchmark
        auto start = std::chrono::steady_clock::now();

        data.block = new UCHAR[ASYNC_SIZE];
        aes::ExpandKey(key, data.key);

        if (mode == LOCK) {

            std::cout << "Encrypting..." << endl;

            // Encrypt in chunks of equal size in parallel
            for (int i = trail; i < file_size; i += ASYNC_SIZE) {
                EncryptAsync(aes::Encrypt, data);
            }

            // Encrypt the remainder
            if (trail) {
                EncryptTrail(data, trail);
            }
        }
        else if (mode == UNLOCK) {

            std::cout << "Decrypting..." << endl;

            for (int i = trail; i < file_size; i += ASYNC_SIZE) {
                EncryptAsync(aes::Decrypt, data);
            }

            if (trail) {
                DecryptTrail(data, trail);
            }
        }

        auto end = std::chrono::steady_clock::now();
        double et = double(std::chrono::duration_cast <std::chrono::milliseconds> (end - start).count());
        std::cout << "Finished in " << et << " ms" << endl;

        data.in_file.close();
        data.out_file.close();

        delete[] data.block;
    }

    return 0;
}
