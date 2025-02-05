#include <fstream>
#include <iostream>
#include <sstream>

void parse(const std::string &input, std::string &inst, std::string &seg,
           std::string &index) {
    std::stringstream ss(input);
    ss >> inst >> seg >> index;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << std::format("Usage: {} <filename>", argv[0]);
        exit(-1);
    }

    std::ifstream in(argv[1], std::ios::in);
    if (!in) {
        std::cerr << "Cannot open file: " << argv[1] << "\n";
        exit(-1);
    }

    std::string pathOut(argv[1]);
    std::string name = pathOut.substr(0,pathOut.find('.'));
    pathOut = name + ".asm";
    size_t t;
    if((t = name.find_last_of('/')) != std::string::npos) {
        name = name.substr(t + 1);
    } else if ((t = name.find_last_of('\\')) != std::string::npos) {
        name = name.substr(t + 1);
    }

    std::ofstream out(pathOut, std::ios::out | std::ios::trunc);
    if (!out) {
        std::cerr << "Cannot open file: " << pathOut << "\n";
        in.close();
        exit(-1);
    }

    std::string s;
    int labelno = 0;
    while (std::getline(in, s)) {
        if (s.find("//") != std::string::npos) {
            continue;
        } else if (s.find_first_not_of(' ') == std::string::npos) {
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
                out << "@.TRUE" << labelno << "\n";
                if(inst == "eq")
                    out << "D;JEQ\n";
                else if (inst == "gt")
                    out << "D;JGT\n";
                else if (inst == "lt")
                    out << "D;JLT\n";
                out << "@SP\n";
                out << "A=M-1\n";
                out << "M=0\n";
                out << "@.END" << labelno << "\n";
                out << "0;JMP\n";
                out << "(.TRUE" << labelno << ")\n";
                out << "@SP\n";
                out << "A=M-1\n";
                out << "M=-1\n";
                out << "(.END" << labelno << ")\n";
                labelno++;
            } 
        } else if (inst == "neg" || inst == "not") {
            out << "@SP\n";
            out << "A=M-1\n";
            if(inst == "neg")
                out << "M=-M\n";
            else
                out << "M=!M\n";
        }
    }
    in.close();
    out.close();
    std::cout << "Complete!\n";
    return 0;
}