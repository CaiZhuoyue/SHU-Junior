//
//  main.cpp
//  操作系统实验四
//
//  Created by 蔡卓悦 on 2021/1/15.
//

#include <iostream>
#include <stdio.h>
#include <string>
#include <cstring>

#define NUM 10
#define MAX_OPEN_NUM 5

using namespace std;


struct MFD_Entry{//主文件目录 目录项
    string UserName;                // 用户名
    struct UFD_Entry *fp;            // UFD 的指针——用于来模拟目录指针
};

struct UFD_Entry{//用户文件目录 目录项
    string FileName;                // 文件名
    int length;                        // 文件长度。用更改文件长度，模拟修改文件。
    bool protectCode[3];            // 文件保护码
    bool isWork;
};

class FileManage
{
private:
    MFD_Entry MFD[NUM];                        // MFD 数组，用于存储用户名
    UFD_Entry *UFD;                            // UFD 指针
    UFD_Entry *openFiles[MAX_OPEN_NUM];        // 打开文件后的，文件指针。（这里简化，直接用 UFD 来模拟）
    int MFD_Length;                            // MFD 目录使用的数量
    int Openfile_Length;                    // 打开文件的数量
    bool isLogin;                            // 标记用户登陆状态
public:

    void init();                            // 初始化函数
    void help();                            // 帮助函数

    void adduser();                            // 添加用户函数
    void deleteuser();                        // 删除用户函数

    void login();                            // 用户登陆函数
    void logout();                            // 用户登出函数

    void createfile();                        // 新建文件函数
    void deletefile();                        // 删除文件函数

    void openfile();                        // 打开文件函数
    void closefile();                        // 关闭文件函数

    void readfile();                        // 读文件函数
    void writefile();                        // 写文件函数
    void changeprotectCode();                // 修改保护码

    void lsfunc();                            // 查看文件

};


//指令字符串数组
char const *commands_str[] = {
    "login",
    "logout",
    "adduser",
    "deleteuser",
    "create",
    "delete",
    "open",
    "close",
    "read",
    "write",
    "ls",
    "change",
    "help"
};


//函数指针数组
void (FileManage::*commands_func[])() = {
    &FileManage::login,
    &FileManage::logout,
    &FileManage::adduser,
    &FileManage::deleteuser,
    &FileManage::createfile,
    &FileManage::deletefile,
    &FileManage::openfile,
    &FileManage::closefile,
    &FileManage::readfile,
    &FileManage::writefile,
    &FileManage::lsfunc,
    &FileManage::changeprotectCode,
    &FileManage::help};


void FileManage::init()
{
    MFD_Length = 0;
    Openfile_Length = 0;
    UFD = NULL;
    isLogin = false;
    for(int i = 0 ;i < NUM; i++)
    {
        MFD[i].UserName = "";
        MFD[i].fp = NULL;
    }

    for(int i = 0; i < MAX_OPEN_NUM; i++)
    {
        openFiles[i] = NULL;
    }
}

void FileManage::adduser()
{
    if(isLogin)
    {
        cout << " Illegal operation! Please log in!" << endl;
        return;
    }
    string name;//用户名
    cout << " Please input the username: ";
    cin >> name;
    // 用于判断是否重名的标志
    bool flag;
    // 用户没有到达10个用户的上限 即主目录表项数目没有到10
    if(MFD_Length >= 0 && MFD_Length <= 10)
    {
        // 遍历MFD 判断用户名是否重名
        for(int i = 0; i < NUM; i++)
        {
            flag = 0;
            if(MFD[i].UserName.compare(name) == 0)
            {
                cout << " The username already exists!" << endl;
                break;
            }
            flag = 1;
        }

        if(flag == 1) //没有重名
        {
            // 创建新的用户
            for(int i = 0; i < NUM; i++)
            {
                if(MFD[i].UserName == "")
                {
                    MFD[i].UserName = name;
                    // 创建，并初始化
                    UFD_Entry *temp;
                    temp = new UFD_Entry[10];
                    // 创建 11 个项 其中第一个项用于储存一些基本信息，不提供用户使用，属于系统层面。
                    for(int j = 0; j <= NUM; j++)
                    {
                        temp[j].FileName = "*********";
                        temp[j].length = 0;
                        temp[j].isWork = false;
                        for(int k = 0; k <3; k++)
                        {
                            temp[j].protectCode[k] = 0;
                        }
                    }

                    MFD[i].fp = temp;
                    cout << " Created!" << endl;
                    MFD_Length++;
                    break;
                }
            }
        }

    }
}

void FileManage::deleteuser()
{
    // 在用户登陆后删除用户操作，不合法，应该禁止
    if(isLogin)
    {
        cout << " Illegal operation! Please logout!" << endl;
        return;
    }
    string name;
    cout << " Please input the username: ";
    cin >> name;

    // 用户判断
    if(MFD_Length > 0 && MFD_Length <= 10)
    {
        // 遍历MFD 寻找指定用户
        for(int i = 0; i < NUM; i++)
        {
            if(MFD[i].UserName.compare(name) == 0)
            {
                MFD[i].UserName = "";
                UFD_Entry *temp = MFD[i].fp;

                delete[] temp;
                MFD[i].fp = NULL;

                MFD_Length--;

                cout << "Deleted!" << endl;
                break;
            }
        }
    }
}

void FileManage::login()
{
    if(isLogin)//重复登录
    {
        cout << " Illegal operation! You already logged in!" << endl;
        return;
    }

    string inBuf, temp;
    temp = " Unsuccessfully！ The usernamse not exist！";
    cout << " Please input the username: ";
    cin >> inBuf;

    // 在 MFD 中查询用户
    for(int i = 0; i < NUM; i++)
    {
        if(MFD[i].UserName.compare(inBuf) == 0)
        {
            // 获取用户目录的目录指针
            UFD = MFD[i].fp;
            isLogin = true;
            temp = "successfully";
            break;
        }
    }

    cout << " Login " << temp << endl;;
}

void FileManage::logout()
{
    if(!isLogin)//没有登陆
    {
        cout << " Illegal operation! Please login!" << endl;
        return;
    }
    UFD = NULL;
    isLogin = false;
    cout << " Bye!" << endl;
}


void FileManage::createfile()//创建文件
{

    if(!isLogin)
    {
        cout << " Illegal operation! Please login!" << endl;
        return;
    }

    string inBuf;
    bool flag;
    cout << " Please input the file's name: " ;
    cin >> inBuf;

    if(UFD[0].length >= 0 && UFD[0].length <=10)
    {
        // 判断文件是否重名
        for(int i = 1; i <= NUM; i++)
        {
            flag = 0;
            if(UFD[i].FileName.compare(inBuf) == 0)
            {
                cout << " The filename already exists!" << endl;
                break;
            }
            flag = 1;
        }

        if(flag == 1)
        {
            for(int i = 1; i <= NUM; i++)
            {
                // 找到空闲区域，创建文件
                if(UFD[i].FileName.compare("*********") == 0)
                {
                    // 辨别系统保留字符串
                    if(inBuf == "*********")
                    {
                        cout << "Please change the filename!" << endl;
                        return;
                    }
                    UFD[i].FileName = inBuf;
                    cout << " The new file is created!" << endl;

                    cout << " Enter the open mode:    ";
                    cin >> inBuf;
                    for(int j =0; j <3;j++)
                    {
                        if(inBuf[j] == '0')
                        {
                            UFD[i].protectCode[j] = false;
                        }
                        else {
                            UFD[i].protectCode[j] = true;
                        }
                    }
                    UFD[0].length++;
                    break;
                }
            }

        }

    }
    else {
        cout << "No free space!" << endl;
    }

}


void FileManage::deletefile()
{
    if(!isLogin)
    {
        cout << " Illegal operation! Please login!" << endl;
        return;
    }
    string inBuf, temp;
    temp = " The file not found!";
    cout << " Please input the file's name: " ;
    cin >> inBuf;

    // 用户判断
    if(MFD_Length > 0 && MFD_Length <= 10)
    {
        // 遍历，寻找指定文件
        for(int i = 1; i <= NUM; i++)
        {
            if(UFD[i].FileName.compare(inBuf) == 0)
            {
                // 检查文件是否占用
                if(UFD[i].isWork == true)
                {
                    cout << " This file is occupied and cannot be deleted " << endl;
                    return;
                }

                UFD[i].FileName = "*********";
                UFD[i].length = 0;
                UFD[0].length--;

                temp = "Deleted File！";
                break;
            }
        }
        cout << temp << endl;
    }
    else {
        cout << "error!" << endl;
    }
}

void FileManage::openfile()
{
    // 用户登陆后，才能进行操作。
    if(!isLogin)
    {
        cout << " Illegal operation! Please login!" << endl;
        return;
    }
    string inBuf;
    bool isFind = false;
    cout << " Pleae input the file's name: ";
    cin >> inBuf;

    if(UFD[0].length > 0 && UFD[0].length <= 10)
    {
        // 遍历，寻找指定文件
        for(int i = 1; i <= NUM; i++)
        {
            if(UFD[i].FileName.compare(inBuf) == 0)
            {
                if(UFD[i].isWork == true)
                {
                    cout << " The file has opened!"<< endl;
                    return;
                }
                // 判断打开文件是否已经到达上限
                if(Openfile_Length>=0 && Openfile_Length < MAX_OPEN_NUM)
                {
                    for(int j = 0; j < MAX_OPEN_NUM; j++)
                    {
                        // 寻找空文件指针
                        if(openFiles[j] == NULL)
                        {
                            UFD[i].isWork = true;
                            // 获取文件指针
                            openFiles[j] = &UFD[i];
                            cout << " The file is opened.It's open num is " << j << "." << endl;
                            isFind = true;
                            break;
                        }
                    }

                    if(isFind == true)
                    {
                        break;
                    }
                }
                else {
                    cout <<  " openFiles error!" << endl;
                }

            }
        }
        if(isFind == false)
        {
            cout << " The file not found!" << endl;
        }
    }
    else
    {
        cout << " error!" << endl;
    }
}


void FileManage::closefile()
{
    if(!isLogin)
    {
        cout << " Illegal operation! Please login!" << endl;
        return;
    }
    int inBuf;
    cout << " Please input the file's open num: ";
    cin >> inBuf;

    // open num 判断有效性，避免指针误操作
    if(inBuf>=0 && inBuf < 5)
    {
        // 解除文件占用
        openFiles[inBuf]->isWork = false;
        openFiles[inBuf] = NULL;
    }
}


void FileManage::readfile()
{
    if(!isLogin)
    {
        cout << " Illegal operation! Please login!" << endl;
        return;
    }

    int inBuf;
    cout << " Please input the file's open num: ";
    cin >> inBuf;

    // open num 判断有效性，避免指针误操作
    if(inBuf>=0 && inBuf < 5)
    {
        if(openFiles[inBuf] != NULL)
        {
            if(openFiles[inBuf]->protectCode[1] == 1)
            {
                cout << " The file length is " << openFiles[inBuf]->length << endl;
            }
            else
            {
                cout << " The file is unreadable!" << endl;
            }
        }
        else
        {
            cout << " File pointer is invalid!" << endl;
        }
    }
}

void FileManage::writefile()
{
    if(!isLogin)
    {
        cout << " Illegal operation! Please login!" << endl;
        return;
    }

    int inBuf1, inBuf2;
    cout << " Please input the file's open num: ";
    cin >> inBuf1;

    // open num 判断有效性，避免指针误操作
    if(inBuf1>=0 && inBuf1 < 5)
    {
        if(openFiles[inBuf1] != NULL)
        {
            if(openFiles[inBuf1]->protectCode[0] == 1)
            {
                cout << " Plese input the length: ";
                cin >> inBuf2;
                if(inBuf2 >= 0)
                {
                    openFiles[inBuf1]->length = inBuf2;
                }
                else
                {
                    cout << " Input error!" << endl;
                }
            }
            else
            {
                cout << " The file is unwritable!" << endl;
            }
        }
        else
        {
            cout << " File pointer is invalid!" << endl;
        }
    }
}

void FileManage::lsfunc()
{
    if(!isLogin)
    {
        cout << " Illegal operation! Please login!" << endl;
        return;
    }

    if(UFD[0].length == 0)
    {
        cout << " Nothing!" << endl;
        return;
    }
    else
    {
        cout << " filename\tmode\tlength" << endl;
    }
    for(int i = 0; i < NUM; i++)
    {
        if(UFD[i].FileName.compare("*********") != 0)
        {
            cout << " " <<UFD[i].FileName << "\t\t" << UFD[i].protectCode[0] << UFD[i].protectCode[1] << UFD[i].protectCode[2] << "\t  " << UFD[i].length << endl;
        }
    }
}


void FileManage::changeprotectCode()
{
    if(!isLogin)
    {
        cout << " Illegal operation! Please login!" << endl;
        return;
    }

    string inBuf;
    bool isfind=false;
    cout << " Please input the file's name: ";
    cin >> inBuf;

    // 用户判断
    if(MFD_Length > 0 && MFD_Length <= 10)
    {
        // 遍历，寻找指定文件
        for(int i = 1; i <= NUM; i++)
        {
            if(UFD[i].FileName.compare(inBuf) == 0)
            {
                isfind = true;

                // 判断更改的文件是否被占用
                if(UFD[i].isWork == true)
                {
                    cout << " This file is occupied and the protectCode cannot be  change!" << endl;
                    return;
                }

                cout << " Enter the open mode:    ";
                cin >> inBuf;
                for(int j =0; j <3;j++)
                {
                    if(inBuf[j] == '0')
                    {
                        UFD[i].protectCode[j] = false;
                    }
                    else {
                        UFD[i].protectCode[j] = true;
                    }
                }
                cout << "It changed!" << endl;
                break;
            }
        }
        if(isfind == false)
        {
            cout << "The file can't find!" << endl;
        }
    }
    else {
        cout << "error!" << endl;
    }
}

void FileManage::help()
{
    cout << " These shell commands are defined internally." << endl;
    cout << " create, delete, open, close, read, write, adduser, deleteuser, ls, change" << endl;
}

int main(int argc, char *argv[]) {
    FileManage filemanage;
    filemanage.init();
    cout << "Hello, you can input 'help' to find the command!" << endl;
    while(1)
    {
        string commandBuf;
        cout << "Pleae input command:  ";
        cin >> commandBuf;

        int len = (sizeof(commands_str) / sizeof(commands_str[0])); //求字符串数组长度
        bool isFind = false;

        for (int i = 0; i < len; i++)
        {
            if (!strcmp(commandBuf.data(), commands_str[i]))
            {
                (filemanage.*commands_func[i])();
                isFind = true;
                break;
            }
        }

        if (!isFind) // 未找到命令
        {
            cout << "command " <<commandBuf << " is not found!" << endl;
        }

    }

    return 0;
}
