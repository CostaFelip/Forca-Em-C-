#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cctype>

using namespace std;

// Função para limpar a palavra (remover pontuação e converter para maiúscula)
string limparPalavra(const string& palavra) {
    string limpa;
    for (char c : palavra) {
        if (isalpha(c)) {
            limpa += toupper(c);
        }
    }
    return limpa;
}

// Função para adicionar nova palavra ao arquivo
void adicionarPalavra() {
    string novaPalavra;
    char adicionar;
    
    cout << "\nDeseja adicionar uma nova palavra ao jogo? (s/n): ";
    cin >> adicionar;
    
    if (adicionar == 's' || adicionar == 'S') {
        cout << "Digite a nova palavra (sem espacos): ";
        cin >> novaPalavra;
        
        string palavraLimpa = limparPalavra(novaPalavra);
        
        if (palavraLimpa.length() >= 4) {
            ofstream arquivo("forca.txt", ios::app); // Abre em modo append
            
            if (arquivo.is_open()) {
                arquivo << "\n" << palavraLimpa;
                arquivo.close();
                
                cout << "\n================================" << endl;
                cout << "Palavra '" << palavraLimpa << "' adicionada com sucesso!" << endl;
                cout << "Ela estara disponivel nos proximos jogos!" << endl;
                cout << "================================" << endl;
            } else {
                cout << "Erro ao adicionar a palavra ao arquivo!" << endl;
            }
        } else {
            cout << "A palavra deve ter pelo menos 4 letras!" << endl;
        }
    }
}

// Função para carregar palavras do arquivo
vector<string> carregarPalavras(const string& nomeArquivo, char dificuldade) {
    ifstream arquivo(nomeArquivo);
    vector<string> palavras;
    
    if (!arquivo.is_open()) {
        cout << "Erro ao abrir o arquivo: " << nomeArquivo << endl;
        cout << "Criando arquivo com palavras padrao..." << endl;
        
        // Criar arquivo com algumas palavras padrão
        ofstream novoArquivo(nomeArquivo);
        if (novoArquivo.is_open()) {
            novoArquivo << "CASA\nGATO\nSOL\nAMOR\nVIDACORRETO\n";
            novoArquivo << "COMPUTADOR\nBRASIL\nESTUDO\nAMIGO\nTRABALHO\n";
            novoArquivo << "PROGRAMACAO\nDESENVOLVIMENTO\nTECNOLOGIA\n";
            novoArquivo << "UNIVERSIDADE\nCONHECIMENTO\nAPRENDIZAGEM\n";
            novoArquivo.close();
            
            // Tentar carregar novamente
            arquivo.open(nomeArquivo);
        }
    }
    
    if (arquivo.is_open()) {
        string palavra;
        while (arquivo >> palavra) {
            string palavraLimpa = limparPalavra(palavra);
            
            // Filtrar por dificuldade
            if (dificuldade == 'F' || dificuldade == 'f') {
                // Fácil: 4-6 letras
                if (palavraLimpa.length() >= 4 && palavraLimpa.length() <= 6) {
                    palavras.push_back(palavraLimpa);
                }
            } else if (dificuldade == 'N' || dificuldade == 'n') {
                // Normal: 7-9 letras
                if (palavraLimpa.length() >= 7 && palavraLimpa.length() <= 9) {
                    palavras.push_back(palavraLimpa);
                }
            } else if (dificuldade == 'D' || dificuldade == 'd') {
                // Difícil: 10+ letras
                if (palavraLimpa.length() >= 10) {
                    palavras.push_back(palavraLimpa);
                }
            }
        }
        arquivo.close();
    }
    
    // Remove duplicatas
    sort(palavras.begin(), palavras.end());
    palavras.erase(unique(palavras.begin(), palavras.end()), palavras.end());
    
    return palavras;
}

// Função para desenhar a forca
void desenharForca(int erros) {
    cout << "\n  +---+" << endl;
    cout << "  |   |" << endl;
    
    if (erros >= 1) cout << "  |   O" << endl; // cabeça
    else cout << "  |    " << endl;
    
    if (erros == 2) cout << "  |   |" << endl; // corpo
    else if (erros == 3) cout << "  |  /|" << endl; // corpo + braço esquerdo
    else if (erros >= 4) cout << "  |  /|\\" << endl; // corpo + ambos braços
    else cout << "  |    " << endl;
    
    if (erros == 5) cout << "  |  /" << endl; // perna esquerda
    else if (erros >= 6) cout << "  |  / \\" << endl; // ambas pernas
    else cout << "  |    " << endl;
    
    cout << "  |" << endl;
    cout << "=====" << endl;
}

// Função para mostrar o progresso da palavra
void mostrarProgresso(const string& palavra, const vector<bool>& descobertas) {
    cout << "\nPalavra: ";
    for (size_t i = 0; i < palavra.length(); i++) {
        if (descobertas[i]) {
            cout << palavra[i] << " ";
        } else {
            cout << "_ ";
        }
    }
    cout << endl;
}

// Função para mostrar letras já tentadas
void mostrarLetrasTentadas(const vector<char>& letrasTentadas) {
    if (!letrasTentadas.empty()) {
        cout << "Letras tentadas: ";
        for (char letra : letrasTentadas) {
            cout << letra << " ";
        }
        cout << endl;
    }
}

int main() {
    srand(time(0));
    char jogarNovamente;
    
    do {
        string dificuldadeStr = "";
        char dificuldade;
        int maxErros = 6;
        
        cout << "\n================================" << endl;
        cout << "    Bem-vindo ao JOGO DA FORCA!" << endl;
        cout << "Adivinhe a palavra antes que seja" << endl;
        cout << "        enforcado!" << endl;
        cout << "================================\n" << endl;
        
        // Escolha da dificuldade
        do {
            cout << "\n================================" << endl;
            cout << "     Escolha Uma Dificuldade:" << endl;
            cout << "(F)Facil (4-6 letras)" << endl;
            cout << "(N)Normal (7-9 letras)" << endl;
            cout << "(D)Dificil (10+ letras)" << endl;
            cout << "================================\n" << endl;
            cin >> dificuldadeStr;
            
            if (dificuldadeStr.length() == 1) {
                dificuldade = dificuldadeStr[0];
            }
            
            if (dificuldade == 'F' || dificuldade == 'f') {
                cout << "Nivel FACIL selecionado!" << endl;
            } else if (dificuldade == 'N' || dificuldade == 'n') {
                cout << "Nivel NORMAL selecionado!" << endl;
            } else if (dificuldade == 'D' || dificuldade == 'd') {
                cout << "Nivel DIFICIL selecionado!" << endl;
            } else {
                cout << "\n================================" << endl;
                cout << "   Opcao invalida! Digite novamente." << endl;
                cout << "================================\n" << endl;
            }
            
        } while (dificuldade != 'F' && dificuldade != 'N' && dificuldade != 'D' && 
                 dificuldade != 'f' && dificuldade != 'n' && dificuldade != 'd');
        
        // Carregar palavras do arquivo
        vector<string> palavras = carregarPalavras("forca.txt", dificuldade);
        
        if (palavras.empty()) {
            cout << "\nErro: Nenhuma palavra encontrada para esta dificuldade!" << endl;
            cout << "Tente uma dificuldade diferente." << endl;
            continue;
        }
        
        // Escolher palavra aleatória
        string palavraSecreta = palavras[rand() % palavras.size()];
        vector<bool> descobertas(palavraSecreta.length(), false);
        vector<char> letrasTentadas;
        int erros = 0;
        bool ganhou = false;
        
        cout << "\nPalavra escolhida! (" << palavraSecreta.length() << " letras)" << endl;
        
        // Loop principal do jogo
        while (erros < maxErros && !ganhou) {
            desenharForca(erros);
            mostrarProgresso(palavraSecreta, descobertas);
            mostrarLetrasTentadas(letrasTentadas);
            
            cout << "\nErros: " << erros << "/" << maxErros << endl;
            cout << "Digite uma letra: ";
            
            char letra;
            cin >> letra;
            letra = toupper(letra);
            
            // Verificar se a letra já foi tentada
            if (find(letrasTentadas.begin(), letrasTentadas.end(), letra) != letrasTentadas.end()) {
                cout << "Voce ja tentou essa letra!" << endl;
                continue;
            }
            
            letrasTentadas.push_back(letra);
            
            // Verificar se a letra existe na palavra
            bool acertou = false;
            for (size_t i = 0; i < palavraSecreta.length(); i++) {
                if (palavraSecreta[i] == letra) {
                    descobertas[i] = true;
                    acertou = true;
                }
            }
            
            if (acertou) {
                cout << "Muito bem! A letra '" << letra << "' esta na palavra!" << endl;
            } else {
                erros++;
                cout << "Ops! A letra '" << letra << "' nao esta na palavra." << endl;
            }
            
            // Verificar se ganhou
            ganhou = true;
            for (bool descoberta : descobertas) {
                if (!descoberta) {
                    ganhou = false;
                    break;
                }
            }
        }
        
        // Resultado final
        desenharForca(erros);
        
        if (ganhou) {
            cout << "\n================================" << endl;
            cout << "    PARABENS! VOCE GANHOU!" << endl;
            cout << "A palavra era: " << palavraSecreta << endl;
            cout << "Voce usou " << letrasTentadas.size() << " tentativas." << endl;
            cout << "================================" << endl;
            
            // Oferecer para adicionar nova palavra
            adicionarPalavra();
        } else {
            cout << "\n================================" << endl;
            cout << "      VOCE FOI ENFORCADO!" << endl;
            cout << "A palavra era: " << palavraSecreta << endl;
            cout << "Mais sorte na proxima vez!" << endl;
            cout << "================================" << endl;
        }
        
        cout << "\nDeseja jogar novamente? (s/n): ";
        cin >> jogarNovamente;
        
    } while (jogarNovamente == 's' || jogarNovamente == 'S');
    
    cout << "\n================================" << endl;
    cout << "   Obrigado por jogar!" << endl;
    cout << "      Ate a proxima!" << endl;
    cout << "================================" << endl;
    
    return 0;
}