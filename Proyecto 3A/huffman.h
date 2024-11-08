#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <vector>
#include <map>
#include <bitset>
#include <algorithm>

// Estructura del nodo para el árbol de Huffman
struct Node {
    unsigned char data;
    unsigned frequency;
    Node* left;
    Node* right;

    Node(unsigned char data, unsigned frequency) {
        this->data = data;
        this->frequency = frequency;
        left = right = nullptr;
    }
};

// Comparador para la cola de prioridad
struct CompareNode {
    bool operator()(Node* l, Node* r) {
        return l->frequency > r->frequency;
    }
};

// Funciones de manejo de bits
unsigned char setBit(unsigned char byte, int position);
unsigned char clearBit(unsigned char byte, int position);
bool getBit(unsigned char byte, int position);

// Funciones principales del algoritmo Huffman
void countFrequency(const std::string& filename, std::map<unsigned char, unsigned>& freq);
Node* buildHuffmanTree(std::map<unsigned char, unsigned>& freq);
void generateCodes(Node* root, std::string code, std::map<unsigned char, std::string>& huffmanCodes);
void compressFile(const std::string& inputFile, const std::string& outputFile);
void decompressFile(const std::string& inputFile, const std::string& outputFile);

// Funciones de utilidad
bool isCompressedFile(const std::string& filename);
std::streamsize getFileSize(const std::string& filename);
void showCompressionStats(const std::string& originalFile, const std::string& compressedFile);

// Función para obtener la extensión de un archivo
std::string getFileExtension(const std::string& filename);

#endif // HUFFMAN_H