#include "tree.h"
#include "model.h"
#include "memory_buffer.h"
#include "file_buffer.h"
#include "arithmetic_compressor.h"
#include <iostream>
#include <chrono>
#include <string>

using namespace std;

void aluno(){
	cout<<"\n\t----------------------------------------------------------";
	cout<<"\n\tINTRODUCAO A TEORIA DA INFORMACAO - ITI (2019.1)";
	cout<<"\n\tALUNO: Helter Yordan";
	cout<<"\n\n\t\t>>>> Trabalho de Implementacao 01 <<<<\n";
	cout<<"\n\n\t    Desenvolver um compressor de arquivos [PPM-C]\n";
	cout<<"\t----------------------------------------------------------\n\n"<<endl;	
}

int main(int argc, char* argv[]){

	//***** CRIAÇÃO DE VARIAVEIS
	string opcao1;
	string opcao2;
	string opcao3;
	int aux = 1;
	int aux2 = 1;

	uint in_size, out_size;
	float RC;
	uchar k;
	uchar header[5] = {0xAB, 0x58, 0x44, 0x47, 0xBB};
	double entropy;
	std::chrono::high_resolution_clock::time_point start_time, end_time;
	//*******************************************************************
	
	//********************* COLETANDO ENTRADA DO USUARIO *****************
	do{
	system ("cls");
	aluno();	
	cout <<"\n\n\tDigite sua opcao:\n\n\t-c = Comprimir\n\t-d = Descomprimir\n\n"; //solicitando a opcao se é para comprimir ou descomprimir
	cout <<"\n\t\t\t9 - SAIR"<<endl;
	cin >> opcao1;
	
	if(opcao1 == "9"){
		return 0;
	}
	
	if ((opcao1 == "-c") || (opcao1 == "-d")){
		
		do{	
		system ("cls");
		aluno();
		cout << "\n\n\tNossa arvore interna pode ser criada de duas formas:\n\n\t-list = listas\n\t-map = mapas\n\n\tInforme a opcao desejada:\n\n"; //solicitando a opcao se é para comprimir ou descomprimir
		cin >> opcao3;
		
		if((opcao3 == "-list") || (opcao3 == "-map")){
			aux2 = 0;
		}
		else{
			system ("cls");
			aluno();
			cout <<"\n\n\tOPCAO INVALIDA\n\n\n";
			system ("pause");
		}
		
		}while(aux2 == 1);
		
		aux = 0;
	}
	
	else{
		system ("cls");
		aluno();
		cout <<"\n\n\tOPCAO INVALIDA\n\n\n";
		system ("pause");
	}
	
	}while(aux == 1);
	
	system ("cls");
	aluno();
	
	cout << "\n\n\tInforme o nome do arquivo:"; //coletando o nome do arquivo
	cin >> opcao2;
	
	system ("cls");
	aluno();
	
	cout << "\n\n\tInforme o Valor de K:"; // coletando o valor de K
	cin >> k;
	
	system ("cls");
	aluno();
	
	//********************************************************************

	string compress = opcao1;
	string input_file_name = opcao2;
	string output_file_name;

//___________________________ [ COMPRIMIR ] _________________________________________________________
	//___________________________________________________________________________________________________
	if(compress == "-c"){

		// ********* EFETUAMOS UMA CHECAGEM SE O ARQRUIVO DE SAIDA JÁ EXISTE, SE JÁ EXISTE ENTÃO SABEMOS QUE JÁ FIZEMOS ESTA COMPRESÃO ANTES
		output_file_name = input_file_name + ".ppm";
		if( ifstream{output_file_name} ){
			cerr << "Arquivo de saida ja existe.\n\n";
			exit(0);
		}
		//****************************************************
		
		//********** INICIAMOS NOSSO PROCESSO DE ACORDO COM A ENTRADA DO USUARIO, SE VAMOS TRABALHAR COM LISTA OU MAPAS
		uint tree_mode = 0;
		if( opcao3 == "-map") tree_mode = 1;
		else if( opcao3 == "-list" ) tree_mode = 2;
		//***************************************************

		// ******** INICIAMOS OS OBJETOS
		FileSymbolBuffer input{input_file_name};
		FileBitBuffer output{output_file_name};
		ArithmeticCompressor comp{k, tree_mode};
		//****************************************************

		// ******** ESCREVENDO O CABEÇALHO COM AS INFORMAÇÕES
		in_size = input.size();
		for(uint i = 0; i < 5; i++)
			output.writeBlock(header[i]);
		output.writeBlock(k);
		output.writeBlock(in_size/8);
		//***************************************************

		// ******** COMPRIMINDO O ARQUIVO
		start_time = std::chrono::high_resolution_clock::now();
		comp.encode(input, output, entropy);
		end_time = std::chrono::high_resolution_clock::now();
		//***************************************************
		
		// ******** [ EMPRESTADO ] CRIA UM CONTADOR PERCENTUAL PARA O PROCESSO
		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
		out_size = output.size();
		RC = (float)in_size / out_size;
		//***************************************************

		// ******** PRINTANDO OS DADOS FINAIS DO PROCESSO DE COMPRESSAO NA TELA
		cerr << std::setprecision(3);
		cerr << "\n\nTempo total: " << elapsed.count()/1000.0f << " s" << std::endl;
		cerr << "\n1) Informacao do arquivo de entrada:" << std::endl;
		cerr << std::setw(15) << std::left << "  Comprimento medio: " << 8.0f << " bits/simbolo" << std::endl;
		cerr << "\n2) Informacoa do arquivo de saida:" << std::endl;
		cerr << std::setw(15) << std::left << "  Comprimento medio: " << (float)out_size/(in_size/8) << " bits/simbolo" << std::endl;
		cerr << "\n3)Informacao da codificacao:" << std::endl;
		cerr << std::setw(15) << std::left << "  Entropia ----> " << entropy << " bits/simbolo" << std::endl;
		cerr << std::setw(15) << std::left << "  RC ----------> " << RC << " : 1" << std::endl;
		//***************************************************
	}
	
	//___________________________ [ DESCOMPRIMIR ] ______________________________________________________
	//___________________________________________________________________________________________________
	else if(compress == "-d"){

		uint num_symbols;
		uchar header_check;

		// ******** CHECAMOS O NOME DO ARQUIVO PARA VER SE É UM ARQUIVO QUE COMPRIMIMOS
		uint s = input_file_name.size();
		if(s > 4 && input_file_name.substr( s-4 ) == ".ppm" )
			output_file_name = input_file_name.substr(0, s-4);
		else{
			std::cerr << "Formato de arquivo inválido. Por favor, insira um arquivo PPM.\n\n";
			exit(0);
		}
		//***************************************************

		
		// ******** CHECAMOS SE JA EXISTE UM ARQUIVO DE SAIDA COM ESSE NOME
		if( argc > 3 ) output_file_name = std::string{argv[3]};
		if( std::ifstream{output_file_name} ){
			std::cerr << "Arquivo de saida ja existe.\n\n";
			exit(0);
		}
		//****************************************************

		//********** INICIAMOS NOSSO PROCESSO DE ACORDO COM A ENTRADA DO USUARIO, SE VAMOS TRABALHAR COM LISTA OU MAPAS
		uint tree_mode = 0;
		if( argc > 4 && std::string{argv[4]} == "-map") tree_mode = 1;
		else if( argc > 4 && std::string{argv[4]} == "-list") tree_mode = 2;
		//****************************************************

		// ******** CRIAMOS OS OBJETOS
		FileBitBuffer input{input_file_name};
		FileSymbolBuffer output{output_file_name};
		//****************************************************

		// ******** VAMOS EXTRAIR AS INFORMAÇÕES DO CABEÇALHO, INFORMAÇÕES QUE COLOCAMOS NO PROCESSO DE COMPRESSÃO
		for(uint i = 0; i < 5; i++){
			input.readBlock(header_check);
			if(header_check != header[i]){
				std::cerr << "Formato de arquivo inválido. Por favor, insira um arquivo PPM.\n\n";
				exit(0);
			}
		}
		input.readBlock(k);
		input.readBlock(num_symbols);

		ArithmeticCompressor comp{k, tree_mode};
		//*****************************************************

		// ******** DESCONPRIMIMOS 
		start_time = std::chrono::high_resolution_clock::now();
		comp.decode(input, output, num_symbols);
		end_time = std::chrono::high_resolution_clock::now();
		//*****************************************************
		
		// ******** [ EMPRESTADO ] CRIA UM CONTADOR PERCENTUAL PARA O PROCESSO
		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
		//*****************************************************

		// ******* INFORMAMOS EM TELA OS DADOS DA DESCOMPRESSAO
		cerr << std::setprecision(3);
		cerr << "\n\nTempo total: " << elapsed.count()/1000.0f << " s" << std::endl;
		//*****************************************************
	}
	std::cerr << "\n";

	return 0;
}
