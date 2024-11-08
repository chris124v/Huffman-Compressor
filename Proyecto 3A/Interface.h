#pragma once

#ifndef INTERFACE_H
#define INTERFACE_H

#include <iostream>
#include <string>

// Enumeración para los colores de la interfaz
enum ConsoleColor {
    RESET = 0,
    CYAN = 36,
    YELLOW = 33,
    GREEN = 32,
    MAGENTA = 35
};

// Estructura para opciones del menú
struct MenuOption {
    int id;
    const char* description;
};

// Constantes del menú
const MenuOption MENU_OPTIONS[] = {
    {1, "Comprimir Archivo"},
    {2, "Descomprimir Archivo"},
    {3, "Informacion"},
    {4, "Salir"}
};

// Funciones de la interfaz
void setColor(ConsoleColor color);
void clearScreen();
void displayLogo();
void displayMenu();
void displayInfo();
void displayProcessing();
void displayError(const std::string& message);
void displaySuccess(const std::string& message);

// Constantes de la interfaz (ASCII Art)
const char* const LOGO_ART = R"(
 ___________              _________ .__                   .___                    
 \__    ___/____   ____  \_   ___ \|  |   _________   __| _/____  ______  ______
   |    |_/ __ \_/ ___\ /    \  \/|  |  /  _ \__  \ / __ |/  _ \/  ___/ /  ___/
   |    |\  ___/\  \___ \     \___|  |_(  <_> ) __ \/ /_/ (  <_> )___ \  \___ \ 
   |____| \___  >\___  > \______  /____/\____(____  \____ |\____/____  >/____  >
              \/     \/         \/                \/     \/          \/      \/ 
)";

const char* const MENU_FRAME = R"(
+----------------------------------------+
|             MENU PRINCIPAL              |
|----------------------------------------|
|  [1] Comprimir Archivo                 |
|  [2] Descomprimir Archivo             |
|  [3] Informacion                      |
|  [4] Salir                            |
+----------------------------------------+
)";

const char* const INFO_FRAME = R"(
+========================================+
|       INFORMACION DEL PROGRAMA         |
|----------------------------------------|
|  * Desarrollado por: Tecficadores      |
|  * Version: 1.0                        |
|  * Algoritmo: Compresion de Huffman    |
|                                        |
|  Tipos de archivos soportados:         |
|  * Sin comprimir: .txt, .bmp, etc.     |
|  * Comprimidos: .exe, .jpg, .mp3, etc. |
|                                        |
|  Presione Enter para continuar...      |
+========================================+
)";

#endif // INTERFACE_H
