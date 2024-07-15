#include <iostream>
#include <fstream>



int main(int argc,char** argv) {
    std::string filename;
    for (int i=1; i<argc;i++){
        std:: string filecheck=argv[i];
        if (filecheck[0]!='-')
        {
            filename=filecheck;
        }

    }
    std::ifstream file(filename);
    int strings=0;
    int words=0;
    std::string line;
    while (not file.eof())
    {
        getline(file,line);
        strings ++;
        for (int i=1; i<line.size();i++)
        {

            if ( line[i-1]!=' ' && line[i]==' ' )
            {
                words++;
            }
        }
        if (line[line.size()-1]!=' '){
            words++;
        }
    }
    std::ifstream file2(filename,std::ios::binary);
    int sizef=strings-1;
    std::string linebin;
    while (not file2.eof()){
        getline(file2,linebin);
        sizef+=linebin.size();
    }
    bool checkcommand=false;
    for (int i=1; i<argc; i++){
        std::string command=argv[i];
        if (command[0]=='-'){
            checkcommand=true;
            if (command[1]=='-'){
                if (command=="--lines")
                {
                    std::cout<<strings<<' ';
                }
                if (command=="--words")
                {
                    std::cout<<words<<' ';
                }
                if (command=="--bytes")
                {
                    std::cout<<sizef<<' ';
                }
            } else{
                for (int j=1;j<command.size();j++){
                    if (command[j]=='l')
                    {
                        std::cout<<strings<<' ';
                    }
                    if (command[j]=='w')
                    {
                        std::cout<<words<<' ';
                    }
                    if (command[j]=='c')
                    {
                        std::cout<<sizef<<' ';
                    }
                }

            }
        }
    }
    if (checkcommand==false)
    {
        std::cout<<strings<<' '<<words<<' '<<sizef<<' '<<filename;
    }

    return 0;
}
