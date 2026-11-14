#include <jni.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>

double firstNum = 0.0;
char currentOp = 0;
bool newInput = true;
bool errorState = false;
std::string displayStr = "0";
std::string exprStr = "";

std::string FormatNumber(double val) {
    if (std::isnan(val) || std::isinf(val)) return "Error";
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(10) << val;
    std::string s = oss.str();
    if (s.find('.') != std::string::npos) {
        s.erase(s.find_last_not_of('0') + 1, std::string::npos);
        if (!s.empty() && s.back() == '.') s.pop_back();
    }
    if (s == "-0" || s.empty()) s = "0";
    return s;
}

extern "C" JNIEXPORT void JNICALL
Java_com_obsidian_calculator_MainActivity_pressButton(JNIEnv* env, jobject, jstring btnCode) {
    const char* raw = env->GetStringUTFChars(btnCode, nullptr);
    std::string cmd(raw);
    env->ReleaseStringUTFChars(btnCode, raw);

    if (errorState && cmd != "C") {
        displayStr = "0";
        errorState = false;
    }

    if (cmd.length() == 1 && cmd[0] >= '0' && cmd[0] <= '9') {
        if (newInput || displayStr == "0") {
            displayStr = cmd;
            newInput = false;
        } else if (displayStr.length() < 15) {
            displayStr += cmd;
        }
    }
    else if (cmd == ".") {
        if (newInput) {
            displayStr = "0.";
            newInput = false;
        } else if (displayStr.find('.') == std::string::npos) {
            displayStr += ".";
        }
    }
    else if (cmd == "C") {
        firstNum = 0.0;
        currentOp = 0;
        newInput = true;
        errorState = false;
        displayStr = "0";
        exprStr = "";
    }
    else if (cmd == "BACK") {
        if (!newInput && !displayStr.empty()) {
            displayStr.pop_back();
            if (displayStr.empty() || displayStr == "-") {
                displayStr = "0";
                newInput = true;
            }
        }
    }
    else if (cmd == "SIGN") {
        if (displayStr != "0" && !displayStr.empty()) {
            if (displayStr.front() == '-') displayStr.erase(0, 1);
            else displayStr = "-" + displayStr;
        }
    }
    else if (cmd == "%") {
        double val = std::stod(displayStr) / 100.0;
        displayStr = FormatNumber(val);
        newInput = true;
    }
    else if (cmd == "+" || cmd == "-" || cmd == "x" || cmd == "/") {
        firstNum = std::stod(displayStr);
        currentOp = cmd[0];
        exprStr = FormatNumber(firstNum) + " " + cmd;
        newInput = true;
    }
    else if (cmd == "=" && currentOp != 0) {
        double secondNum = std::stod(displayStr);
        double result = 0.0;
        std::string opStr(1, currentOp);
        exprStr = FormatNumber(firstNum) + " " + opStr + " " + FormatNumber(secondNum) + " =";

        if (currentOp == '+') result = firstNum + secondNum;
        else if (currentOp == '-') result = firstNum - secondNum;
        else if (currentOp == 'x') result = firstNum * secondNum;
        else if (currentOp == '/') {
            if (secondNum != 0.0) result = firstNum / secondNum;
            else errorState = true;
        }

        if (errorState) displayStr = "Cannot divide by 0";
        else displayStr = FormatNumber(result);

        currentOp = 0;
        newInput = true;
    }
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_obsidian_calculator_MainActivity_fetchDisplay(JNIEnv* env, jobject) {
    return env->NewStringUTF(displayStr.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_obsidian_calculator_MainActivity_fetchExpr(JNIEnv* env, jobject) {
    return env->NewStringUTF(exprStr.c_str());
}

