#include<iostream>
#include"huffmantrie.hpp"
#include<fstream>
#include<iostream>
#include <string>
#include<sstream>
#include<queue>
#include"FrenquenceTable.hpp"
using namespace std;
int main(int argc,char* argv[])
{
    // Handle command line arguments
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " InputFile OutputFile" << std::endl;
        return -1;
    }
    string inputFile(argv[1]);
    string outputFile = (argv[2]);
    try {


        std::ifstream infstream(inputFile);// input text file
        std::ofstream outfstream(outputFile, ios::binary);// output as a binary file

        stringstream buffer;
        buffer << infstream.rdbuf();
        string inputChars(buffer.str());
        infstream.close();

        cout << endl << "The input text is " << inputChars.size() << " bytes." << endl;

        HFCodeTree hftree;//creat humantree
        string codedtext = hftree.Compress(inputChars);
        hftree.WriteComprssedFile(outfstream, codedtext);//write compressed file to output
        outfstream.close();

        /// ///////////////////////////


        /********************************************************/
        /****reat compressed file again, construct tree from file and decode the file*/
        infstream.open(outputFile, ios::binary);
        string codedtextfromfile = hftree.ReadCompressedFile(infstream);

        infstream.close();

        cout<<endl<<codedtext<< endl;
        auto decodeds = hftree.decode(codedtextfromfile);
        cout << decodeds << endl;
    } catch (std::exception& e) {
        printf( "%s", e.what());
    }
  /***********************************************************/
    return 0;
}