#include <iostream>
#include <string>
#include <vector>
enum TokenType{Plus,Minus,Mul,Div,Number,Endchar};
enum State{H,NUM,ZerNum};
enum ReadTokenResult{OK,END,INVALID_CHAR};
struct Token{
    TokenType type;
    std::string volume;
    Token(){
        type=Endchar;
        volume='\0';
    }
};
struct Tokenizer{
    State state;
    std::string& str;
    Tokenizer(std::string& s):str(s){
        state = H;
    }
    ReadTokenResult ReadToken(Token& token);
};
ReadTokenResult Tokenizer::ReadToken(Token& token){
    token.volume.clear();
    state = H;
    while(!str.empty()){
        const char c=str.front();
        str.erase(str.begin());
        switch(state){
            case H:
                if(c==' '){
                    break;
                }
                if(c=='\0'){
                    return END;
                }
                if(c>='1' && c<='9'){
                    state=NUM;
                    token.volume.push_back(c);
                }else{
                    //To filter numbers starting with 0
                    if (c=='0'){
                        state=ZerNum;
                        token.volume.push_back(c);
                    }else {
                        switch (c) {
                            case '+':
                                token.type = Plus;
                                break;
                            case '-':
                                token.type = Minus;
                                break;
                            case '*':
                                token.type = Mul;
                                break;
                            case '/':
                                token.type = Div;
                                break;
                            default:
                                return INVALID_CHAR;
                        }
                        token.volume.push_back(c);
                        return OK;
                    }
                }
                break;
            case NUM:
                if(c>='0' && c<='9'){
                    token.volume.push_back(c);
                }else{
                    token.type=Number;
                    str.insert(str.begin(), c);
                    return OK;
                }
                break;
            case ZerNum:
                if(c>='1' && c<='9'){
                    return INVALID_CHAR;
                }else {
                    token.type=Number;
                    str.insert(str.begin(), c);
                    return OK;
                }
        }
    }
    return END;
}
struct Execute{
    const std::vector<Token>& token_mass;
    Token token;
    int a, b, i;
    std::vector<int> res_mass;
    Execute(const std::vector<Token>& mass):token_mass(mass){
        token=token_mass[0];
	i = 1;        
	a = 0;
        b = 0;
        res_mass.clear();
    }
    void GetToken();
    void Small_expr();
    void Add();
    void Mul();
    void Numb();
    int Check_string();
};
void Execute::GetToken() {
    token=token_mass.at(i);
    i++;
}
int Execute::Check_string() {
    Small_expr();
    if(token.type != Endchar || res_mass.size() != 1) {
        throw "Incorrect expression";
    }
    return res_mass.back();
}
void Execute::Small_expr() {
    Add();
    std::string op;
    while(token.volume=="-"||token.volume=="+"){
        op = token.volume;
        GetToken();
        Add();
        if(res_mass.size() >= 2) {
            a = res_mass.back();
            res_mass.pop_back();
            b = res_mass.back();
            res_mass.pop_back();
            if (op == "+") {
                res_mass.push_back(a + b);
            } else {
                res_mass.push_back(b - a);
            }
        }else{
            throw "Expected number";
        }
    }
}
void Execute::Add(){
    Mul();
    std::string op;
    while(token.volume=="*"||token.volume=="/"){
        op = token.volume;
        GetToken();
        Mul();
        if(res_mass.size() >= 2){
            a = res_mass.back();
            res_mass.pop_back();
            b = res_mass.back();
            res_mass.pop_back();
            if(op=="*"){
                res_mass.push_back(a*b);
            }else{
                if(a != 0){
                    res_mass.push_back(b/a);
                }else{
                    throw "Div by 0";
                }
            }
        }else{
            throw "Expected number";
        }
    }
}
void Execute::Mul() {
    //Unary operations
    std::string op;
    if(token.volume=="-"||token.volume=="+"){
        op = token.volume;
        GetToken();
    }
    Numb();
    if(op == "-"){
        a = res_mass.back();
        res_mass.pop_back();
        res_mass.push_back(a*(-1));
    }
}
void Execute::Numb() {
    if(token.type==Number){
        res_mass.push_back(stoi(token.volume));
        GetToken();
    }else{
        throw "Expected number";
    }
}
int main(int argc, char* argv[])
{
    std::string my_string(argv[1]);
    my_string += '\0';
    Tokenizer f(my_string);
    Token token;
    std::vector<Token> token_mass;
    ReadTokenResult res;
    while((res = f.ReadToken(token))==OK){
        token_mass.push_back(token);
    }
    if(res==INVALID_CHAR){
        std::cout<<"Incorrect expression";
        return -1;
    }
    token.type = Endchar;
    token.volume = '\0';
    token_mass.push_back(token);
    int res1;
    try{
        Execute execute(token_mass);
        res1 = execute.Check_string();
    }
    catch(std::out_of_range&){
        std::cout<<"Error std::out_of_range";
        return -1;
    }
    catch (const char* str){
        std::cout<<str;
        return -1;
    }
    std::cout << res1;
    return 0;
}
