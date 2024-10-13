#include "blockchain.h"
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>

Block::Block(int index, const std::string& previousHash, const std::string &data)
        :index(index), previousHash(previousHash), data(data), hash(calculateHash()) {}

int Block::getIndex() const {
    return index; 
}

const std::string& Block::getPreviousHash() const {
    return previousHash; 
}

const std::string& Block::getData() const {
    return data; 
}

const std::string& Block::getHash() const {
    return hash; 
}

std::string Block::calculateHash() const {
    std::stringstream ss; 
    ss << index << previousHash <<data; 
    std::string input = ss.str(); 

    unsigned char hash[SHA256_DIGEST_LENGTH]; 
    SHA256(reinterpret_cast<const unsigned char *>(input.c_str()), input.size(), hash); 

    std::stringstream hashStream; 
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        hashStream << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(hash[i]); 
    }

    return hashStream.str(); 
}

Blockchain::Blockchain() : blockCount(0) {
    // create Genesis Block the first block on the block chain 
    chain.emplace_back(0, "0", "Genesis Block"); 
}

void Blockchain::addBlock(const std::string& data) {
    std::string previousHash = chain.back().getHash(); 
    chain.emplace_back(++blockCount, previousHash, data); 
}

Block Blockchain::getBlock(int index) const {
    if (index < 0 || index > blockCount) {
        throw std::out_of_range("Block index out of range"); 
    }

    reutrn chain[index]; 
}

std::string Blockchain::getBlockchainInfo() const {
    std::stringstream info; 
    info << "Blockchain contains " << blockCount + 1 << " blocks:\n"; 
    for (const auto& block : chain) {
        info << "Block " << block.getIndex() << " [Hash: " << block.getHash()
             << ", Previous Hash : " << block.getPreviousHash() 
             << ", Data: " << block.getData() << "] \n"; 
    }

    return info.str(); 
}

int Blockchain::getBlockCount() const {
    return blockCount + 1; // includes the Genesis Block 
}
