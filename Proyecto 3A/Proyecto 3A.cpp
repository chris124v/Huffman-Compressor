// Proyecto 3A.cpp 
// Christopher Daniel Vargas Villalta, Carnet: 2024108443
// Santiago Espinoza Rendon, Carnet: 2024156530

#include "huffman.h"
#include "interface.h"

// Implementación de funciones de manejo de bits
unsigned char setBit(unsigned char byte, int position) {
    return byte | (1 << position);
}

unsigned char clearBit(unsigned char byte, int position) {
    return byte & ~(1 << position);
}

bool getBit(unsigned char byte, int position) {
    return (byte >> position) & 1;
}

// Implementación de funciones de interfaz
void setColor(ConsoleColor color) {
    std::cout << "\033[1;" << color << "m";
}

void clearScreen() {
    system("cls");
}

void displayLogo() {
    clearScreen();
    setColor(CYAN);
    std::cout << LOGO_ART << std::endl;
    setColor(RESET);
}

void displayMenu() {
    setColor(YELLOW);
    std::cout << MENU_FRAME;
    setColor(RESET);
}

void displayInfo() {
    clearScreen();
    setColor(GREEN);
    std::cout << INFO_FRAME;
    setColor(RESET);
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void displayProcessing() {
    setColor(MAGENTA);
    std::cout << "\nProcesando... Por favor espere...\n" << std::endl;
    setColor(RESET);
}

void displayError(const std::string& message) {
    std::cout << "\n[ERROR] " << message << std::endl;
}

void displaySuccess(const std::string& message) {
    std::cout << "\n[EXITO] " << message << std::endl;
}

// Función para obtener la extensión de un archivo
std::string getFileExtension(const std::string& filename) {
    size_t pos = filename.find_last_of(".");
    if (pos != std::string::npos) {
        return filename.substr(pos);
    }
    return "";
}

// Implementación de funciones del algoritmo Huffman
void countFrequency(const std::string& filename, std::map<unsigned char, unsigned>& freq) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("No se pudo abrir el archivo: " + filename);
    }

    unsigned char byte;
    while (file.read(reinterpret_cast<char*>(&byte), 1)) {
        freq[byte]++;
    }
    file.close();
}

Node* buildHuffmanTree(std::map<unsigned char, unsigned>& freq) {
    std::priority_queue<Node*, std::vector<Node*>, CompareNode> pq;

    for (const auto& pair : freq) {
        pq.push(new Node(pair.first, pair.second));
    }

    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();

        Node* parent = new Node('\0', left->frequency + right->frequency);
        parent->left = left;
        parent->right = right;
        pq.push(parent);
    }

    return pq.empty() ? nullptr : pq.top();
}

void generateCodes(Node* root, std::string code, std::map<unsigned char, std::string>& huffmanCodes) {
    if (!root) return;

    if (!root->left && !root->right) {
        huffmanCodes[root->data] = code;
    }

    generateCodes(root->left, code + "0", huffmanCodes);
    generateCodes(root->right, code + "1", huffmanCodes);
}

void compressFile(const std::string& inputFile, const std::string& outputFile) {
    std::map<unsigned char, unsigned> freq;
    countFrequency(inputFile, freq);

    Node* root = buildHuffmanTree(freq);
    if (!root) return;

    std::map<unsigned char, std::string> huffmanCodes;
    generateCodes(root, "", huffmanCodes);

    std::ifstream inFile(inputFile, std::ios::binary);
    std::ofstream outFile(outputFile, std::ios::binary);

    unsigned mapSize = static_cast<unsigned>(freq.size());
    outFile.write(reinterpret_cast<char*>(&mapSize), sizeof(mapSize));

    for (const auto& pair : freq) {
        outFile.write(reinterpret_cast<const char*>(&pair.first), sizeof(pair.first));
        outFile.write(reinterpret_cast<const char*>(&pair.second), sizeof(pair.second));
    }

    std::string bits;
    unsigned char byte;
    while (inFile.read(reinterpret_cast<char*>(&byte), 1)) {
        bits += huffmanCodes[byte];
    }

    unsigned char paddingBits = 8 - (bits.length() % 8);
    if (paddingBits == 8) paddingBits = 0;
    outFile.write(reinterpret_cast<char*>(&paddingBits), sizeof(paddingBits));

    for (size_t i = 0; i < bits.length(); i += 8) {
        unsigned char compressedByte = 0;
        for (int j = 0; j < 8 && i + j < bits.length(); ++j) {
            if (bits[i + j] == '1') {
                compressedByte = setBit(compressedByte, 7 - j);
            }
        }
        outFile.write(reinterpret_cast<char*>(&compressedByte), 1);
    }

    inFile.close();
    outFile.close();
}

void decompressFile(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream inFile(inputFile, std::ios::binary);
    std::ofstream outFile(outputFile, std::ios::binary);

    unsigned mapSize;
    inFile.read(reinterpret_cast<char*>(&mapSize), sizeof(mapSize));

    std::map<unsigned char, unsigned> freq;
    for (unsigned i = 0; i < mapSize; ++i) {
        unsigned char character;
        unsigned frequency;
        inFile.read(reinterpret_cast<char*>(&character), sizeof(character));
        inFile.read(reinterpret_cast<char*>(&frequency), sizeof(frequency));
        freq[character] = frequency;
    }

    Node* root = buildHuffmanTree(freq);
    if (!root) return;

    unsigned char paddingBits;
    inFile.read(reinterpret_cast<char*>(&paddingBits), sizeof(paddingBits));

    Node* current = root;
    unsigned char byte;
    while (inFile.read(reinterpret_cast<char*>(&byte), 1)) {
        for (int i = 7; i >= 0; --i) {
            bool bit = getBit(byte, i);

            if (bit) {
                current = current->right;
            }
            else {
                current = current->left;
            }

            if (!current->left && !current->right) {
                outFile.write(reinterpret_cast<char*>(&current->data), 1);
                current = root;
            }
        }
    }

    inFile.close();
    outFile.close();
}

bool isCompressedFile(const std::string& filename) {
    std::string extension = getFileExtension(filename);
    std::vector<std::string> compressedExtensions = {
        ".exe", ".jpg", ".jpeg", ".png", ".mp3", ".mp4", ".zip", ".rar", ".gz", ".7z"
    };
    return std::find(compressedExtensions.begin(), compressedExtensions.end(), extension) != compressedExtensions.end();
}

std::streamsize getFileSize(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    return file.tellg();
}

void showCompressionStats(const std::string& originalFile, const std::string& compressedFile) {
    std::streamsize originalSize = getFileSize(originalFile);
    std::streamsize compressedSize = getFileSize(compressedFile);

    double compressionRatio = (1.0 - static_cast<double>(compressedSize) / originalSize) * 100;

    setColor(CYAN);
    std::cout << "\nEstadisticas de compresion:" << std::endl;
    setColor(RESET);
    std::cout << "Tamano original: " << originalSize << " bytes" << std::endl;
    std::cout << "Tamano comprimido: " << compressedSize << " bytes" << std::endl;
    std::cout << "Ratio de compresion: " << compressionRatio << "%" << std::endl;
}

int main() {
    std::string dataPath = "C:\\Users\\Christopher\\OneDrive\\Documents\\Notas";

    while (true) {
        displayLogo();
        displayMenu();
        std::cout << "Seleccione una opcion: ";

        int option;
        std::cin >> option;
        std::cin.ignore();

        if (option == 4) break;

        if (option == 3) {
            displayInfo();
            continue;
        }

        if (option != 1 && option != 2) {
            displayError("Opcion invalida");
            continue;
        }

        clearScreen();
        displayLogo();

        std::string filename;
        std::cout << "Ingrese el nombre del archivo (con extension): ";
        std::getline(std::cin, filename);

        std::string inputFile = dataPath + filename;

        try {
            if (option == 1) {
                displayProcessing();
                if (isCompressedFile(inputFile)) {
                    std::cout << "\nAdvertencia: El archivo ya esta comprimido. La compresion adicional podria no ser efectiva.\n" << std::endl;
                }

                std::string outputFile = inputFile + ".huf";
                compressFile(inputFile, outputFile);
                showCompressionStats(inputFile, outputFile);
                displaySuccess("Archivo comprimido guardado como: " + outputFile);
            }
            else if (option == 2) {
                if (getFileExtension(filename) != ".huf") {
                    displayError("El archivo debe tener extension .huf");
                    continue;
                }

                displayProcessing();
                std::string outputFile = inputFile.substr(0, inputFile.length() - 4) + "_decompressed" +
                    getFileExtension(filename.substr(0, filename.length() - 4));
                decompressFile(inputFile, outputFile);
                displaySuccess("Archivo descomprimido guardado como: " + outputFile);
            }
        }
        catch (const std::exception& e) {
            displayError(e.what());
        }

        std::cout << "\nPresione Enter para continuar...";
        std::cin.get();
    }

    return 0;
}

