#include <sys/stat.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

void parse(const std::string &input, std::string &inst, std::string &seg,
           std::string &index) {
    std::stringstream ss(input);
    ss >> inst >> seg >> index;
}

void write(std::ifstream &in, std::ofstream &out, std::string &name) {
    std::string s;
    int labelno = 0, retno = 0;
    std::string funcName = "";
    while (std::getline(in, s)) {
        size_t slash;
        if ((slash = s.find("//")) != std::string::npos) {
            s = s.substr(0, slash);
        }
        if (s.find_first_not_of(' ') == std::string::npos) {
            continue;
        }

        out << "// " << s << "\n";
        std::string inst = "", seg = "", index = "";
        parse(s, inst, seg, index);
        if (inst == "push") {
            std::string seg_;
            if (seg == "constant") {
                out << "@" << index << "\n";
                out << "D=A\n";
                out << "@SP\n";
                out << "A=M\n";
                out << "M=D\n";
                out << "@SP\n";
                out << "M=M+1\n";
                continue;
            }

            if (seg == "local") {
                seg_ = "LCL";
            } else if (seg == "argument") {
                seg_ = "ARG";
            } else if (seg == "this") {
                seg_ = "THIS";
            } else if (seg == "that") {
                seg_ = "THAT";
            } else if (seg == "static") {
                seg_ = name + "." + index;
            } else if (seg == "temp") {
                seg_ = "R" + std::to_string(5 + std::stoi(index));
            } else if (seg == "pointer") {
                if (index == "0")
                    seg_ = "THIS";
                else
                    seg_ = "THAT";
            }
            out << "@" << seg_ << "\n";
            if (seg != "static" && seg != "temp" && seg != "pointer") {
                out << "D=M\n";
                out << "@" << index << "\n";
                out << "A=D+A\n";
            }
            out << "D=M\n";
            out << "@SP\n";
            out << "A=M\n";
            out << "M=D\n";
            out << "@SP\n";
            out << "M=M+1\n";
        } else if (inst == "pop") {
            std::string seg_;
            if (seg == "local") {
                seg_ = "LCL";
            } else if (seg == "argument") {
                seg_ = "ARG";
            } else if (seg == "this") {
                seg_ = "THIS";
            } else if (seg == "that") {
                seg_ = "THAT";
            } else if (seg == "static") {
                seg_ = name + "." + index;
            } else if (seg == "temp") {
                seg_ = "R" + std::to_string(5 + std::stoi(index));
            } else if (seg == "pointer") {
                if (index == "0")
                    seg_ = "THIS";
                else
                    seg_ = "THAT";
            }
            if (seg != "static" && seg != "temp" && seg != "pointer") {
                out << "@" + seg_ + "\n";
                out << "D=M\n";
                out << "@" + index + "\n";
                out << "D=D+A\n";
                out << "@R13\n";
                out << "M=D\n";
                out << "@SP\n";
                out << "AM=M-1\n";
                out << "D=M\n";
                out << "@R13\n";
                out << "A=M\n";
                out << "M=D\n";
            } else {
                out << "@SP\n";
                out << "AM=M-1\n";
                out << "D=M\n";
                out << "@" + seg_ + "\n";
                out << "M=D\n";
            }
        } else if (inst == "add" || inst == "sub" || inst == "and" ||
                   inst == "or" || inst == "eq" || inst == "gt" ||
                   inst == "lt") {
            out << "@SP\n";
            out << "AM=M-1\n";
            out << "D=M\n";
            out << "@SP\n";
            out << "A=M-1\n";
            if (inst == "add")
                out << "M=D+M\n";
            else if (inst == "sub")
                out << "M=M-D\n";
            else if (inst == "and")
                out << "M=D&M\n";
            else if (inst == "or")
                out << "M=D|M\n";
            else {
                out << "D=M-D\n";
                out << "@" << funcName << "$TRUE" << labelno << "\n";
                if (inst == "eq")
                    out << "D;JEQ\n";
                else if (inst == "gt")
                    out << "D;JGT\n";
                else if (inst == "lt")
                    out << "D;JLT\n";
                out << "@SP\n";
                out << "A=M-1\n";
                out << "M=0\n";
                out << "@" << funcName << "$END" << labelno << "\n";
                out << "0;JMP\n";
                out << "(" << funcName << "$TRUE" << labelno << ")\n";
                out << "@SP\n";
                out << "A=M-1\n";
                out << "M=-1\n";
                out << "(" << funcName << "$END" << labelno << ")\n";
                labelno++;
            }
        } else if (inst == "neg" || inst == "not") {
            out << "@SP\n";
            out << "A=M-1\n";
            if (inst == "neg")
                out << "M=-M\n";
            else
                out << "M=!M\n";
        } else if (inst == "label") {
            out << "(" << funcName << "$" << seg << ")\n";
        } else if (inst == "goto") {
            out << "@" << funcName << "$" << seg << "\n";
            out << "0;JMP\n";
        } else if (inst == "if-goto") {
            out << "@SP\n";
            out << "AM=M-1\n";
            out << "D=M\n";
            out << std::format("@{}${}\n", funcName, seg);
            out << "D;JNE\n";
        } else if (inst == "function") {
            funcName = seg;
            labelno = 0;
            retno = 0;
            out << std::format("({})\n", funcName);
            for (int i = 0; i < std::stoi(index); i++) {
                out << "@0\n";
                out << "D=A\n";
                out << "@SP\n";
                out << "A=M\n";
                out << "M=D\n";
                out << "@SP\n";
                out << "M=M+1\n";
            }
        } else if (inst == "call") {
            // push @returnAddr, LCL, ARG, THIS, THAT
            out << std::format("@{}$ret.{}\n", funcName, retno);
            out << "D=A\n"
                << "@SP\n"
                << "A=M\n"
                << "M=D\n"
                << "@SP\n"
                << "M=M+1\n";
            for (auto s : {std::string("LCL"), std::string("ARG"),
                           std::string("THIS"), std::string("THAT")}) {
                out << std::format("@{}\n", s);
                out << "D=M\n"
                    << "@SP\n"
                    << "A=M\n"
                    << "M=D\n"
                    << "@SP\n"
                    << "M=M+1\n";
            }

            // ARG = SP-5-nArgs
            out << "@SP\n"
                << "D=M\n"
                << "@5\n"
                << "D=D-A\n"
                << "@" << index << "\n"
                << "D=D-A\n"
                << "@ARG\n"
                << "M=D\n";

            // LCL = SP
            out << "@SP\n"
                << "D=M\n"
                << "@LCL\n"
                << "M=D\n";

            // goto functionName
            out << "@" << seg << "\n"
                << "0;JMP\n";

            // (returnAddr)
            out << std::format("({}$ret.{})\n", funcName, retno);
            retno++;
        } else if (inst == "return") {
            // retAddr = *(endFrame - 5)
            out << "@LCL\n"
                << "D=M\n"
                << "@5\n"
                << "A=D-A\n"
                << "D=M\n"
                << "@R14\n"
                << "M=D\n";

            // *ARG = pop()
            out << "@SP\n";
            out << "AM=M-1\n";
            out << "D=M\n";
            out << "@ARG\n";
            out << "A=M\n";
            out << "M=D\n";

            // SP = ARG + 1
            out << "@ARG\n"
                << "D=M+1\n"
                << "@SP\n"
                << "M=D\n";

            // THAT = *(endFrame - 1)
            out << "@LCL\n"
                << "D=M\n"
                << "@1\n"
                << "A=D-A\n"
                << "D=M\n"
                << "@THAT\n"
                << "M=D\n";
            // THIS = *(endFrame - 2)
            out << "@LCL\n"
                << "D=M\n"
                << "@2\n"
                << "A=D-A\n"
                << "D=M\n"
                << "@THIS\n"
                << "M=D\n";
            // ARG = *(endFrame - 3)
            out << "@LCL\n"
                << "D=M\n"
                << "@3\n"
                << "A=D-A\n"
                << "D=M\n"
                << "@ARG\n"
                << "M=D\n";
            // LCL = *(endFrame - 4)
            out << "@LCL\n"
                << "D=M\n"
                << "@4\n"
                << "A=D-A\n"
                << "D=M\n"
                << "@LCL\n"
                << "M=D\n";

            // goto retAddr
            out << "@R14\n"
                << "A=M\n"
                << "0;JMP\n";
        }
    }
    in.close();
}

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << std::format("Usage: {} <filename | directory>", argv[0]);
        exit(-1);
    }

    std::filesystem::path path(argv[1]);
    if (std::filesystem::is_regular_file(path)) {
        std::ifstream in(path, std::ios::in);
        std::string name = path.stem().string();
        std::filesystem::path outFile = path.replace_extension("asm");
        std::ofstream out(outFile, std::ios::out | std::ios::trunc);
        write(in, out, name);
        out.close();
    } else if (std::filesystem::is_directory(path)) {
        std::filesystem::path outFile = path / "out.asm";
        outFile.replace_filename(outFile.parent_path().filename());
        outFile.replace_extension("asm");
        std::ofstream out(outFile, std::ios::out | std::ios::trunc);
        out << "@Sys.init\n"
            << "0;JMP\n";
        for (auto dir_entry : std::filesystem::directory_iterator(path)) {
            if (dir_entry.path().extension() == ".vm") {
                std::ifstream in(dir_entry.path(), std::ios::in);
                std::string name = dir_entry.path().stem().string();
                write(in, out, name);
            }
        }
        out.close();
    } else {
        throw "Invalid input!\n";
    }

    std::cout << "Complete!\n";
    return 0;
}