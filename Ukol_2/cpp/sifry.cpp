
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

bool answerChecker(string question, string accept, string decline) { // funkce na ano / ne otázky s ošetřením
    string response = "";
    do {
        cout << question;
        cin >> response;
    } while (response != accept && response != decline);

    return response == accept ? true : false;
}

string writeFileContent(string fileName) { // funkce na získání textu co chce uživatel vložit do souboru
    string input = "", rem = "";
    cout << "To end writing, write 'save_file" << "'\n"; // check na ukončení psaní
    do { 
        rem = "";
        getline(cin, rem);
        if (rem != "save_file") {
            input.append(rem + "\n");
        }
    } while (rem != "save_file");
    return input;
}

void addToFile(string fileName, string text) { // funkce pro vložení textu do souboru
    ofstream file; 
    file.open(fileName);
    file << text;
    file.close();

}

string createFile() { // funkce pro vytvoření souboru, vrací název souboru pro další úpravy
    reset: // point pro nevhodně zadaný název souboru
    string fileName = "";
    string fileNameChecker = ".txt";
    int checkIndex = 3;
    ofstream file;
    do {
        fileName = "";
        cout << "Enter file name: ";
        cin >> fileName;
        if (fileName.length() < 4 || fileName[0] == '.') { // ošetření aby jméno nebylo krítké, i když to není nutnost. eliminace . na začátku souboru
            goto reset;
        }
        else {
            for (int i = fileName.length() - 1; i > fileName.length() - 4; i--) { // ošetření pro vhodnou koncovku souboru, buď to doplnění nebo ignorace
                if (fileName[i] != fileNameChecker[checkIndex]) {
                    fileName += fileNameChecker;
                    break;
                }
                checkIndex--;
            }
        };
        file.open(fileName); // kontrola vytvoření souboru
    } while (!file.is_open());
    file.close();
    cout << "File " << fileName << " created sucessfully.\n"; // info 
    return fileName;
    
    
}

string getFileLocation() { // funkce která vyhledává soubor s možností vytvoření a naplnění nového souboru
    bool createFileCheck = false;
    string fileName = "";
    ifstream file;

newFind:
    cout << "Enter file location and name: ";
    cin >> fileName;
    file.open(fileName);
    while (!file) { // pokud nejde otevřít -> nelze najít
        createFileCheck = answerChecker("File not found, do you want to create new file (1) or search again (0)? ( 1 / 0 ): ", "1", "0");
        if (createFileCheck) {
           return createFile(); // vytvoření souboru -> získání jména souboru
        }
        else {
            goto newFind; // opakování vyhledávání
        }

    }
    file.close();
    return fileName;
}

string getFileContents(string fileName) {
    string fullText = "";
    string text = "";
    ifstream readFile(fileName);
    while (getline(readFile, text)) {
        fullText += text + "\n";
    }
    if (fullText.empty()) {
        cout << "File is empty!\n";
        fullText = writeFileContent(fileName);
        addToFile(fileName, fullText);
    }

    return fullText;
}

void caesar(bool encrypt) {
    // caps 65 - 90 | norm 97 - 122
    string offsetString = "";
    int offset = 0;
    int rem = 0;
    bool check = true;
    string fileName = getFileLocation();
    string modifiedFileName;
    string text = getFileContents(fileName);
    string textModified = text;
    do { // získání číselného posunu
        cout << "Enter offset: ";
        cin >> offsetString;
        check = true;
        for (int i = 0; i < offsetString.length(); i++) {
            if (!isdigit(offsetString[i])) {
                check = false;
            }
        }
    } while (!check);
    offset = stoi(offsetString); // převod na int kvůli podmínkám
    if (encrypt) { // šifrování
        for (int i = 0; i < text.length(); i++) {
            rem = (int)text[i] + offset;
            if ((int)text[i] >= 65 && (int)text[i] <= 90) { // caps písmena + ignorace ostatních znaků, isupper() by taky šlo 
                textModified[i] = (((int)text[i] + offset - 65) % 26) + 65; // char na indexu --> na int hodnotu + přidám posun, výsledek dělím počtem písmen abecedy, získávám zbytek odkud dostanu hodnotu 0-25 kterou následně přičtu k int vyjádření 'A' --> přepíšu posunuté písmeno na původní index,
            }
            else if ((int)text[i] >= 97 && (int)text[i] <= 122) { // lowercase písmena + ignorace ostatních znaků
                textModified[i] = (((int)text[i] + offset - 97) % 26) + 97;
            }
        }
        modifiedFileName = fileName.insert(fileName.length() - 4, "Cae"); // úprava jména
        cout << "File encrypted and named as: " << modifiedFileName << "\n"; // info
        ofstream encryptedFile(modifiedFileName);
        addToFile(modifiedFileName, textModified); // uložení obsahu

        cout << "Original file: \n" << text << "\n";
        cout << "Encrypted file: \n" << textModified << "\n";

    }
    else {
        for (int i = 0; i < text.length(); i++) {
            rem = (int)text[i] + offset;
            if ((int)text[i] >= 65 && (int)text[i] <= 90) { // caps písmena + ignorace ostatních znaků, isupper() by taky šlo 
                textModified[i] = (((int)text[i] - offset  - 90) % 26)  + 90; 
            }
            else if ((int)text[i] >= 97 && (int)text[i] <= 122) { // lowercase písmena + ignorace ostatních znaků
                textModified[i] = (((int)text[i] - offset  - 122) % 26) + 122;
            }
        }
        modifiedFileName = fileName.insert(fileName.length() - 4, "Dec");
        cout << "File decrypted and named as: " << modifiedFileName << "\n";
        ofstream encryptedFile(modifiedFileName);
        addToFile(modifiedFileName, textModified);

        cout << "Original file: \n" << text << "\n";
        cout << "Decrypted file: \n" << textModified << "\n";
    }

}

void vigenere(bool encrypt) {
    bool check = true;
    string fileName = getFileLocation(); // získání názvu a umístění souboru, případné vytvoření a naplnění nového
    string input = getFileContents(fileName);; // načtení obsahu souboru;
    string key = "";
    int keyFix = 0;
    string output = input;
    string modifiedFileName = "";
    int rem = 0;
    int inputIndex = 0, keyIndex = 0;

    do { // získání klíče
        cout << "Enter key: ";
        cin >> key;
        check = true;
        for (int i = 0; i < key.length(); i++) {
            if (!isalpha(key[i])) { // iterace stringu a kontrola zda se jedná o charakter abecedy
                check = false;
            }
        }
    } while (!check);

    if (encrypt) {
        do {
            if (isupper(input[inputIndex])) { // Caps písmena
                if (isupper(key[keyIndex])) { // óšetření int hodnoty klíče
                    keyFix = 0;
                }
                else {
                    keyFix = -32;
                }
                rem = ((int)output[inputIndex] - 65); // získání posunu o X které se přidá ke klíči
                if (rem + (int)key[keyIndex] + keyFix > 90) { // přechod přes 'Z'
                    output[inputIndex] = (rem + (int)key[keyIndex] + keyFix - 26); // získání zašifrované hodnoty na indexu s přechodem, vedle bez
                }
                else {
                    output[inputIndex] = (rem + (int)key[keyIndex] + keyFix);
                }
                inputIndex++; // inkrementace indexu -> další char z inputu
                if (keyIndex == key.length() - 1) { // reset klíče
                    keyIndex = 0;
                }
                else { // posun indexu klíče
                    keyIndex++;
                }
            }
            else if (islower(input[inputIndex])) { // to stejné, jako předchozí šifrování, akorát pro lowercase input
                if (isupper(key[keyIndex])) {
                    keyFix = 32;
                }
                else {
                    keyFix = 0;
                }
                rem = ((int)output[inputIndex] - 97);
                if (rem + (int)key[keyIndex] + keyFix > 122) {
                    output[inputIndex] = (rem + (int)key[keyIndex] + keyFix - 26);
                }
                else {
                    output[inputIndex] = (rem + (int)key[keyIndex] + keyFix);
                }
                inputIndex++;
                if (keyIndex == key.length() - 1) {
                    keyIndex = 0;
                }
                else {
                    keyIndex++;
                }
            }
            else { // vynechaní a zachování ostatních znaků které nejsou součástí ang. abecedy
                inputIndex++;
            }
        } while (inputIndex < input.length()); // opakování a postupné šifrování dokud nejsou iterovány všechny hodnoty v inputu dle délky

        modifiedFileName = fileName.insert(fileName.length() - 4, "Vig"); // modifikace jména souboru
        cout << "File encrypted and named as: " << modifiedFileName << "\n"; 
        ofstream encryptedFile(modifiedFileName); // uložení modifikovaného souboru na stejnou cestu jako originální
        addToFile(modifiedFileName, output); // volání funkce která vloží text do modifikovaného souboru

        cout << "Original file: \n" << input << "\n"; // výpis originálu
        cout << "Encrypted file: \n" << output << "\n"; // výpis modif. 
    }
    else { // Rozšifrování funguje stejně jako zašifrování, akorát prohazuji key s inputem, neguji keyFix, odečítám rem od inputu oproti šifrování kde je rem pozitivní posun od klíče, zde je rem posun dozadu od klíče k inputu
        do {
            if (isupper(input[inputIndex])) { // Caps písmena
                if (isupper(key[keyIndex])) {
                    keyFix = 0;
                }
                else {
                    keyFix = +32;
                }
                rem = ((int)key[keyIndex] - 65);
                if ((int)output[inputIndex] + keyFix - rem < 65) {
                    output[inputIndex] = ((int)output[inputIndex] - rem + keyFix + 26);
                }
                else {
                    output[inputIndex] = ((int)output[inputIndex] - rem + keyFix);
                }
                inputIndex++;
                if (keyIndex == key.length() - 1) {
                    keyIndex = 0;
                }
                else {
                    keyIndex++;
                }
            }
            else if (islower(input[inputIndex])) {
                if (isupper(key[keyIndex])) {
                    keyFix = -32;
                }
                else {
                    keyFix = 0;
                }
                rem = ((int)key[keyIndex] - 97);
                if ((int)output[inputIndex] + keyFix - rem < 97) {
                    output[inputIndex] = ((int)output[inputIndex] - rem + keyFix + 26);
                }
                else {
                    output[inputIndex] = ((int)output[inputIndex] - rem + keyFix);
                }
                inputIndex++;
                if (keyIndex == key.length() - 1) {
                    keyIndex = 0;
                }
                else {
                    keyIndex++;
                }
            }
            else { // vynechaní ostatních znaků
                inputIndex++;
            }
        } while (inputIndex < input.length());

        modifiedFileName = fileName.insert(fileName.length() - 4, "Dec");
        cout << "File decrypted and named as: " << modifiedFileName << "\n";
        ofstream encryptedFile(modifiedFileName);
        addToFile(modifiedFileName, output);

        cout << "Original file: \n" << input << "\n";
        cout << "Decrypted file: \n" << output << "\n";
    }

    
}

void XOR() { // mocnina char na indexu x inputu, char. klíče, pro šifrování a dešifrování se využívá stejná funkce se stejným klíčem
    string fileName = getFileLocation(); // získání názvu a umístění souboru, případné vytvoření a naplnění nového
    string input = getFileContents(fileName);; // načtení obsahu souboru;
    string original = input;
    string key = "";
    string modifiedFileName = fileName;
    bool check = true;
    int keyIndex = 0;
        do { // získání klíče
            cout << "Enter key: ";
            cin >> key;
            check = true;
            for (int i = 0; i < key.length(); i++) {
                if (!isalpha(key[i])) { // iterace stringu a kontrola zda se jedná o charakter abecedy
                    check = false;
                }
            }
        } while (!check || key.empty());

        for (int i = 0; i < input.length() - 1; i++) {
            input[i] = input[i] ^ key[keyIndex];
            if (keyIndex == key.length() - 1) {
                keyIndex = 0;
            }
            else {
                keyIndex++;
            }
        }
        if (!fileName.find("XOR.txt")) {
            modifiedFileName = fileName.insert(fileName.length() - 4, "XOR");
        }
        cout << "File Encrypted and named as: " << modifiedFileName << "\n";
        ofstream encryptedFile(modifiedFileName);
        addToFile(modifiedFileName, input);

        cout << "Original file: \n" << original << "\n";
        cout << "Encrypted file: \n" << input << "\n";

        cout << input; 
}

int main()
{

    bool encrypt = answerChecker("Choose to encrypt or decrypt file: (1/0): ", "1", "0"); // výběr šifrování / dešifrování
    string method;
    do {
        cout << "\nChoose which method to use: (1): Caesar   |   (2): Vigenere   |   (3): XOR: "; // výběr funkce
        cin >> method;
    } while (method != "1" && method != "2" && method != "3");

    switch (method[0]) {
    case '1': caesar(encrypt);
        break;
    case '2': vigenere(encrypt);
        break;
    case '3': XOR();
        break;

    }
}
