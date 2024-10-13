#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <string>
#include <vector>
#include <unordered_map>

class Block {
public:
    Block(int index, const std::string& previousHash, const std::string& data);

    int getIndex() const; 
    const std::string& getPreviousHash() const; 
    const std::string& getData() const; 
    const std::string& getHash() const; 

private:
    int index; // block index 
    std::string previousHash; // pre-block's hash 
    std::string data; // current block data content 
    std::string hash; // current block hash value 

    std::string calculateHash() const; // current partition's hash value 
}; 

class Blockchain {
public:
    Blockchain(); 

    void addBlock(const std::string& data); // add a new block to the chain 
    Block getBlock(int index) const; // retrieve a block by giving index 
    std::string getBlockchainInfo() const; // get metadata info of current block
    int getBlockCount() const; // retrieve current chain's block number 

private:
    std::vector<Block> chain; // the instance of the Block chain 
    int blockCount; // current chain block counter 
}; 
#endif