#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include <curses.h>
//#include<conio.h>
//#include<dos.h>
#define keynum 10
#define getspace(type) (type*)malloc(sizeof(type))
using namespace std;

char cmd[64];  //存放用户输入命令
char buffer[36];//
char user[32];//存放当前登陆的用户名

typedef char ALFA[12];
ALFA KWORD[keynum];
struct UFD{//用户文件管理模块
	char filename[32]; //文件名
	int  safecode;     //文件保护码
	long length;       //文件长度
}*curfile = NULL;

struct MFD{//用户登陆信息管理模块
	char username[32]; //用户名
	bool filepoint;    //用户目录下的文件指针，false表示目录为空
}*curuser = NULL,*elseuser=NULL;
typedef UFD UFD;
typedef MFD MFD;
int main();

void KeyWord()//初始化命令关键字
{
	strcpy(KWORD[ 1],"bye");    strcpy(KWORD[ 2],"chmod");
	strcpy(KWORD[ 3],"close");	strcpy(KWORD[ 4],"create"); 
	strcpy(KWORD[ 5],"delete");	strcpy(KWORD[ 6],"list");   
	strcpy(KWORD[ 7],"open");   strcpy(KWORD[ 8],"read");   
	strcpy(KWORD[ 9],"write");
	
}
int LoginDisplay() //登陆选项操作函数
{
	int SELETE_1 = 0;
	do
	{
    cout<<"1、用户登陆  2、用户注册  0、退出"<<endl;
    cin>>SELETE_1;
	}while(SELETE_1<0 || SELETE_1>2);
	system("cls");
	return SELETE_1;
}

bool Login(int SELETE)//用户登陆，注册函数
{
	FILE *fp,*fp1,*fp2;
	char name[12];
	switch(SELETE)
	{
	case 1://用户登陆
		if((fp = fopen("LOGIN.exe","rb")) == NULL)//打开用户注册目录管理文件
		{
			cout<<"\n错误：不能打开登陆文件。"<<endl;
			getch();system("cls");
			return false;
		}
		curuser = getspace(MFD);
		cout<<"\n*****登陆*****\n用户名：";
		cin>>name; //输入用户登陆名
		
		while(!feof(fp)) //检查该用户是否合法
		{
			fread(curuser,sizeof(MFD),1,fp);
			if(strcmp(curuser->username,name)==0)
				break;
		}
		if(feof(fp)) //如果没有找到跟当前登陆用户名相同的管理信息，提示出错
		{
			cout<<"\n错误：该用户不存在。"<<endl;
           	fclose(fp);
	       	return false;
		}
		else
		{
			fclose(fp);
			return true;
		}
		break;
		
	case 2:  //新用户注册
		if((fp=fopen("LOGIN.exe","ab"))==NULL)//如果登陆信息管理文件不存在
			fp=fopen("LOGIN.exe","wb+");      //创建该信息管理文件
		char name[12];
		curuser = getspace(MFD);
		while(1)
		{
			cout<<"*****新用户注册*****"<<endl;
			cout<<"用户名：";
	   		cin>>name;       //输入用户注册名
			fp1 = fopen("LOGIN.exe","rb");
			while(!feof(fp1))//查看该用户名是否被别的用户占用
			{
				fread(curuser,sizeof(MFD),1,fp1);
				if(strcmp(curuser->username,name) == 0)//该名称已经被使用
				{
					cout<<"该用户已经存在，请重新输入！"<<endl;
					getch();
					break;
				}
			}
			if(feof(fp1))//该名称没有被别的用户占用
			{
				strcpy(curuser->username,name);
	       		curuser->filepoint = NULL;
	           	fwrite(curuser,sizeof(MFD),1,fp);
				strcpy(user,curuser->username);//生成用户文件管理模块
				strcat(user,".exe");           //用于管理用户目录下的各个文件
				fp2=fopen(user,"wb+");
				fclose(fp2);
       			cout<<"注册成功！"<<endl;   //提示注册成功
				fclose(fp1);
	   	    	fclose(fp);
				break;
			}
		}
		fp = fopen("LOGIN.exe","rb"); //显示当前注册用户的名称
		while(1)
		{
			fread(curuser,sizeof(MFD),1,fp);
			if(feof(fp))
				break;
			cout<<curuser->username<<endl;
			getch();
		}
		fclose(fp);
		return true;
		break;
	default:
		return false;
		break;
	}
}

void DisplayUFD()//打印用户信息，包括用户的各个文件
                 //名称、长度和操作权限的设置信息
{
	if(curuser->filepoint == false)//当前用户目录下没有任何文件存在
		cout<<"\n用户 "<<curuser->username<<" 文件夹是空的"<<endl;
	else
	{//存在文件，将所有文件信息打印在终端
		FILE *fp;
		char filename[12];
		strcpy(filename,curuser->username);
		strcat(filename,".exe");
		if((fp=fopen(filename,"rb"))==NULL)//打开用户文件信息管理模块
		{
			cout<<"\n无法打开用户："<<curuser->username<<" 的文件！"<<endl;
			getch();
			return;
		}
		else
		{//读入并将用户全部文件信息打印在终端
			cout<<"用户："<<curuser->username<<"目录下的文件："<<endl;
			UFD *ufd;
			int i=0;
			ufd = getspace(UFD); //申请存放用户文件模块的空间
			while(1)
			{			
				fread(ufd,sizeof(UFD),1,fp);
				if(feof(fp))//全部输出完毕，结束
					break;
				else//打印信息
					cout<<ufd->filename<<"\t"<<ufd->length<<"\t"<<ufd->safecode<<endl;
			}
		}
		fclose(fp);
	}
}

void ByeFile(bool BOOL)//注销函数，调用次函数用户可以退出系统
{
	FILE *infile,*outfile;
	char out[50];
	strcpy(out,"outfilelocate.exe");
	if((infile=fopen("LOGIN.exe","rb"))==NULL)
	{
		cout<<"\n保存错误。";	//	fclose(infile);
		return;
	}
	else
	{
		if((outfile=fopen(out,"wb+"))==NULL)//申请一个缓冲区管理模块
			//存放用户更新后的全部信息
		{
			cout<<"\n保存错误。";// fclose(outfile);
			fclose(infile);return;
		}
		else
		{
			MFD *mfd = getspace(MFD);
			while(1)
			{//将旧文件管理信息读出，并保存到新的文件信息管理模块中
				fread(mfd,sizeof(MFD),1,infile);
				if(feof(infile))
					break;
				if((strcmp(mfd->username,curuser->username))==0)
				{
					if(BOOL)//更新当前用户信息的操作
						fwrite(curuser,sizeof(MFD),1,outfile);
					else continue;//如果用户想把自己的注册目录从系统中彻底删除
					//则执行该操作
				}
				else
					fwrite(mfd,sizeof(MFD),1,outfile);//写入新的模块
			}
			fclose(infile);fclose(outfile);
			remove("LOGIN.exe");//将旧的该用户的文件管理模块删除
			rename(out,"LOGIN.exe");//将新的用户的文件管理模块重命名为用户目录下的管理模块	
		}	
	}
	system("cls");
	main();
}

bool ClearUserFile()//用户要将自己的注册目录从系统彻底删除
//首先将该用户目录下的全部文件删除
{
	FILE *fp;
	char file[50];
	strcpy(file,curuser->username);
	strcat(file,".exe");
	if((fp=fopen(file,"rb"))==NULL) //打开用户文件信息管理模块
	{
	//	fclose(fp);
		cout<<"\n操作失败。";return true;
	}
	else
	{//将该用户目录下的文件逐个从磁盘删除
		UFD *ufd = getspace(UFD);
		while(1)
		{
			fread(ufd,sizeof(UFD),1,fp);
			if(feof(fp))
				break;
			else
				remove(ufd->filename);//删除文件
		}
		fclose(fp);
		return true;
	}
}

void ClearUserMes()//删除用户全部信息
{
	char name[50];
	strcpy(name,curuser->username);
	strcat(name,".exe");
	remove(name); //从磁盘中删除用户文件信息管理模块
	ByeFile(false);//更新系统的用户登陆信息管理模块
}
void DeleteUser()//删除用户注册目录的操作
{
	char ch;
cout<<"\n该操作将会是你在系统所有信息删除，下次登陆时你必须重新申请用户名！"<<endl;
	cout<<"\n你确定要删除你在系统中的注册信息吗？Y/N"<<endl;
	cin>>ch;
	switch(ch)//提示用户确认删除
	{
	case 'Y':
		if(ClearUserFile())//如果用户的全部文件已经删除了
			//则可以将该用户的文件信息管理模块也从磁盘中删除
			//以免在没完全删除文件却删了该文件信息管理模块
			//使得这些文件无法再进行管理造成磁盘空间的浪费
			ClearUserMes();//删除文件信息管理模块
		break;
	default:
		cout<<"\n你取消了此操作！";
		break;
	}
}

void CreatFile()//在当前用户目录下创建文件
{
	FILE *fp;
   	curuser->filepoint=true;

	if((fp=fopen(buffer,"r"))==NULL)//如果没有跟用户输入文件名相同的文件
	{
    	if((fp=fopen(buffer,"w"))==NULL)
		{
    		cout<<"\n创建文件失败！";
		//	fclose(fp);
    		return;
		}
		fclose(fp);
	}
	else
	{//用户要创建的文件已经存在
		cout<<"\n该文件已经存在，创建另一个文件？Y/N";
		char ch;
		cin>>ch;
		switch(ch)
		{
		case 'Y':
		case 'y':
			cout<<"\n输入新文件名：";
			cin>>buffer;
			strcat(buffer,".txt");
			fclose(fp);
        	if((fp=fopen(buffer,"w"))==NULL)
			{
        		cout<<"\n创建文件失败！";
			//	fclose(fp);
            	return;
			}
        	fclose(fp);
			break;
		default:
			fclose(fp);
			return;
		}
	}
	strcpy(user,curuser->username);
	strcat(user,".exe");
	curfile = getspace(UFD);
	strcpy(curfile->filename,buffer);//文件名
	curfile->length=0; //该文件长度为零
	curfile->safecode=30; //设置该文件的默认权限
	    //11 00，文件主有读和写权，其他用户没有读写权
	if((fp=fopen(user,"ab"))==NULL)
	{
		cout<<"\n错误：你可能不是合法用户。"<<endl;
		getch();
	}
	else
	{
		fwrite(curfile,sizeof(UFD),1,fp);//将该文件信息写入用户文件信息管理模块中
		cout<<"\n文件 "<<curfile->filename<<" 创建成功！";
	}
	fclose(fp);
}

void DeleteFile()//删除当前目录下一个文件的操作
{
	char ch;
	FILE *infile,*outfile;
	cout<<"\n确定要删除文件："<<buffer<<" Y/N"<<endl;
	cin>>ch;//提示用户确认删除
	switch(ch)
	{
	case 'Y':
	case 'y'://更新用户文件信息管理模块，这里同样使用缓冲区模块来更新
		    //方法与上面将到的类似
			char out[50],in[50];
         	strcpy(out,"outfilelocate.exe");
			strcpy(in,curuser->username);
			strcat(in,".exe");
        	if((infile=fopen(in,"rb"))==NULL)//打开该用户的文件信息管理模块
			{
	        	cout<<"\n保存错误。";
	        	//fclose(infile);
				return;
			}
        	else
			{
	        	if((outfile=fopen(out,"wb+"))==NULL)
				{
	        		cout<<"\n保存错误。";// fclose(outfile);
	        		fclose(infile);return;
				}
	        	else
				{
	        		UFD *ufd = getspace(UFD);
		        	while(1)
					{
		        		fread(ufd,sizeof(UFD),1,infile);//从旧模块读出信息
		        		if(feof(infile))
			        		break;
			        	if((strcmp(ufd->filename,buffer))==0)//要进行更新的信息
							continue;
			        	else
				        	fwrite(ufd,sizeof(UFD),1,outfile);//写入新模块
					}
		        	fclose(infile);fclose(outfile);
		        	remove(in);//在磁盘移除就模块
		        	rename(out,in);	//新模块命名为当前用户文件信息管理模块
				}	
			}
		remove(buffer);//从磁盘中删除该文件
		break;
	default:
		break;
	}
}

void ListAllFile()//显示当前用户目录下的文件信息
{
	DisplayUFD();
}

void OpenFile()//在window模式下打开该文件
{
	system(buffer);//buffer为文件名，如:file1.txt
}

bool QueryModElse(bool BOOL,bool &flag)//查询其它用户目录下文件的文件
//当该文件的权限允许当前用户对其执行有关操作时，返回ture
{
	FILE *fp;
	char user[50];
	UFD *ufd = getspace(UFD);//elseuser表示除当前用户外的所有用户注册目录
	strcpy(user,elseuser->username);
	strcat(user,".exe");
	if((fp=fopen(user,"rb"))==NULL){//打开一个其它的用户文件信息管理模块
	//	fclose(fp);
		cout<<"\n操作出现错误，对此我们表示歉意！";return false;
	}
	else{
		while(1){
			fread(ufd,sizeof(UFD),1,fp);
			if(feof(fp)){
				fclose(fp);return false;
			}
			if(strcmp(ufd->filename,buffer)==0){
				if(BOOL)//该用户请求写该文件
				{
					if(ufd->safecode== 31 || ufd->safecode== 33)
						//1101、1111最后一位为1，有写权
						return true;
					else{
						cout<<"\n你无权对文件 "<<buffer<<" 执行此操作！";
						flag=true;return false;}
					//flag设置为true，告诉上一层，无须再查找
					//该文件已经找到，但用户无权执行相关操作
				}
				else //该用户请求读权
				{
					if(ufd->safecode == 32 || ufd->safecode == 33)
						//1110、1111倒数第二位为1，有读权
						return true;
					else{
						cout<<"\n你无权对文件 "<<buffer<<" 执行此操作！";
						flag=true;return false;}  
				}
			}
		}
	}
}

bool QueryMod(bool BOOL)//查询权限
{//首先在用户目录下查找，如果找不到用户当前要进行操作的文件名
	//则在其它注册用户目录下查找
	FILE *fp,*fp1;
	bool flag=false;
	char user[50];
	UFD *ufd = getspace(UFD);
	strcpy(user,curuser->username);
	strcat(user,".exe");

	if((fp=fopen(user,"rb"))==NULL){//打开用户文件信息管理模块
	//	fclose(fp);
		cout<<"\n操作出现错误，对此我们表示歉意！";return false;
	}
	else{//查找匹配的文件名，用户目录下的文件允许用户进行读写操作
		while(1){
			fread(ufd,sizeof(UFD),1,fp);
			if(feof(fp)){//在当前用户文件管理模块中找不到匹配文件
				//则继续在其它用户注册目录下查找
				fclose(fp);
				fp1=fopen("LOGIN.exe","rb");
				elseuser = getspace(MFD);
				bool BOOL_1=false;
				while(1){					
					fread(elseuser,sizeof(MFD),1,fp1);//读其它用户信息
					if(feof(fp1) && !BOOL_1)//全部用户都查找完
						//但仍然没找到匹配的文件
						return false;
					if(elseuser != curuser){
					   if((BOOL_1=QueryModElse(BOOL,flag)))//查找
						   return true;
					   if(flag)
						   return false;
					}
				}
			}
			if(strcmp(ufd->filename,buffer)==0){//在当前用户注册目录下
				//找到该文件，返回真值
				fclose(fp);return true;
			}
		}
	}
}

bool WriteRight(int len,bool BOOL)//查看是否已经正确地写入到该文件信息中
//是则返回真值
{
	
	char user[50],outfile[50];
	FILE *fp,*fp1;
	strcpy(user,elseuser->username);
	strcat(user,".exe");
	if((fp=fopen(user,"rb"))==NULL){
			return false;
	}
	else{
		UFD *ufd = getspace(UFD);
		while(1){//在此用户目录下查找匹配文件
			fread(ufd,sizeof(UFD),1,fp);
			if(feof(fp)){
				fclose(fp);return false;
			}
			if((strcmp(ufd->filename,buffer))==0){//找到要写入新的长度的文件
				
				strcpy(outfile,"outfilelocate.exe");
				if((fp1=fopen(outfile,"wb+"))==NULL){
					cout<<"\n错误：写入文件长度出错_3。";
				//	fclose(fp1);
					fclose(fp);return false;
				}
				else{
					fclose(fp);
			fp=fopen(user,"rb");//文件指针从新指向此用户文件信息管理模块开头
					while(1){
						fread(ufd,sizeof(UFD),1,fp);
						if(feof(fp))
							break;
						if(strcmp(ufd->filename,buffer)==0){//找到匹配的文件
							if(BOOL) ufd->length+=len; //在文件末追加内容的操作
							else ufd->length =len;     //覆盖原文件内容
						}
						fwrite(ufd,sizeof(UFD),1,fp1);
					}
					fclose(fp);fclose(fp1);
					remove(user);
					rename(outfile,user);
					return true;
				}
			}
		}
	}
}
void WriteLengthToFile(int Len,bool BOOL)//将文件长度写入文件管理模块中
{//因为当前用户可以对其它用户的文件进行操作（只要权限允许）
	//所以应该在整个文件系统目录下查找该文件的位置
	FILE *fp;
	if((fp=fopen("LOGIN.exe","rb"))==NULL){//不能打开文件
		cout<<"\n写入文件长度错误_1！";
	//	fclose(fp);
		return;
	}
	else{
		elseuser = getspace(MFD);
		while(1){
			fread(elseuser,sizeof(MFD),1,fp);
			if(feof(fp))
				break;
			else{
				if(WriteRight(Len,BOOL)){//查看是否已经正确地写入到该文件信息中
					fclose(fp);return;
				}
			}
		}
		cout<<"\n写入文件长度错误_2！";
		fclose(fp);return;
	}
}

void WriteFile()//向文件写入信息的操作
{
	if(!QueryMod(true))//查询当前用户对该文件是否有写权
		return;//对该文件没有写权则返回
	char ch;
	int i=0;
	FILE *fp;
	if((fp=fopen(buffer,"r"))==NULL)//查询该文件是否存在
	{
		cout<<"\n该文件不存在，请创建该文件后再写入。";
	//	fclose(fp);
		return;
	}
	fclose(fp);
	cout<<"\n请选择写入方式："<<endl;
	cout<<"  1、覆盖原文件    2、在原文件末尾写入   3、取消"<<endl;
	cin>>ch;
	cout<<"开始输入正文："<<endl;
	switch(ch)
	{
	case '1'://覆盖原文件
		if((fp=fopen(buffer,"w"))==NULL)
			cout<<"\n文件打开失败。";
		else
		{
			ch=getchar();
			while(ch!='#')//将新的文件内容写入到文件的磁盘位置中
			{
				i++;
				fputc(ch,fp);
				ch=getchar();
			}
		}
		fclose(fp);
		WriteLengthToFile(i,false);//将文件长度写入文件管理模块
		break;
	case '2':
		if((fp=fopen(buffer,"a"))==NULL)
			cout<<"\n文件打开失败。";
		else
		{
			ch=getchar();
			while(ch!='#')//将新的文件内容写入到文件的磁盘位置中
			{
				i++;
				fputc(ch,fp);
				ch=getchar();
			}
		}
		fclose(fp);
		WriteLengthToFile(i,true);//将文件长度写入文件管理模块
		break;
	default:
		break;
	}
}

void ReadFile()//读文件函数
{
	if(!QueryMod(false))//查询当前用户是否有权读该文件
		return;//没有读权，则返回
	FILE *fp;
	if((fp=fopen(buffer,"r"))==NULL)//打开该文件
	{
		cout<<buffer;
		cout<<"\n该文件不存在。";
		return;
	}
	else{
		char ch;
		ch=fgetc(fp);
		while(ch!=EOF)//将该文件信息逐一输出到终端
		{
			putchar(ch);
			ch=fgetc(fp);
		}
		cout<<endl;
	}	
	fclose(fp);
}

void ChangeMod()//修改某文件的执行权限
{
	int mod=40;	
	FILE *fp,*infile,*outfile;

	char in[50],out[50];
	UFD *ufd = getspace(UFD);
	strcpy(in,curuser->username);
	strcat(in,".exe");
	strcpy(out,"outfilelocate.exe");

	if((fp=fopen(in,"rb"))==NULL){
	//	fclose(fp);
		cout<<"\n操作出现错误，对此我们表示歉意！";return;
	}
	else{
		while(1){//查看该文件是否在当前用户的注册目录下
			//任何用户无权修改不是自己目录下的文件的权限值
			fread(ufd,sizeof(UFD),1,fp);
			if(feof(fp)){//在当前目录下找不到该文件，说明该用户无权修改该文件权限
				cout<<"\n你没有权限对文件 "<<buffer<<" 执行该操作！";
				fclose(fp);return;
			}
			if(strcmp(ufd->filename,buffer)==0){//找到该文件，继续操作
				fclose(fp);break;
			}
		}
	}
	bool flag1=true;
	while(flag1)
	{
    	cout<<"\n输入文件  "<<buffer<<" 的新的权限值：";
    	cin>>mod;//输入权限值
		if(mod<30 || mod>33)
		{//确保输入的权限值正确
			cout<<"\n错误：权限值必须在30~33之间";
			continue;
		}
		else{
			char ch;
			switch(mod){//告诉用户对该文件权限修改的结果，以便用户确认
			case 30:
			cout<<"\n当前权限设置：其他用户对"<<buffer<<"既没读权也没写权！";
				break;
			case 31:
				cout<<"\n当前权限设置：其他用户对"<<buffer<<"没读权但有写权！";
				break;
			case 32:
				cout<<"\n当前权限设置：其他用户对"<<buffer<<"有读权但没写权！";
				break;
			case 33:
			cout<<"\n当前权限设置：其他用户对"<<buffer<<"既有读权也有写权！";
				break;
			default: break;
			}
			cout<<"\n确认按'Y'，取消按'N':";
			cin>>ch;
			switch(ch){
			case 'Y':
			case 'y':flag1=false;break;
			default: flag1=true;
			}
		}
	}
	//更新文件信息管理模块，相关操作类似上面，不在赘述
	if((infile=fopen(in,"rb"))==NULL){
		cout<<"\n操作出现错误，对此我们表示歉意！";fclose(infile);
		return;
	}
	else{
		if((outfile=fopen(out,"wb+"))==NULL){
	    	cout<<"\n操作出现错误，对此我们表示歉意！";
			fclose(infile);//fclose(outfile);
	    	return;
		}
		else{
			while(1)
			{
				fread(ufd,sizeof(UFD),1,infile);
        		if(feof(infile))
	        		break;
		       	if((strcmp(ufd->filename,buffer))==0)
					ufd->safecode=mod;
	        	fwrite(ufd,sizeof(UFD),1,outfile);
			}
		    fclose(infile);fclose(outfile);
		   	remove(in);
		   	rename(out,in);	
		}
	}
}
void Execute(int i,int len,int cmdset)//执行命令函数
{
	int j=0;
	for(;i<len;i++)
	{		
		if(cmd[i]=='>'||cmd[i]==' ')
			break;
		buffer[j]=cmd[i];j++;
	}
	buffer[j]='\0';
	strcat(buffer,".txt");
	switch(cmdset)
	{
	case 1: //退出
		ByeFile(true);
		break;
	case 2: //改变文件操作权限
		if((strcmp(buffer,".txt"))==0){
			cout<<"\n输入命令出错！";
			return;
		}
		ChangeMod();
		break;
	case 3: //删除用户
        DeleteUser();
		break;
	case 4: //创建文件
		if((strcmp(buffer,".txt"))==0){
			cout<<"\n输入命令出错！";
			return;
		}
		CreatFile();
		break;
	case 5: //删除文件
		if((strcmp(buffer,".txt"))==0){
			cout<<"\n输入命令出错！";
			return;
		}
		DeleteFile();
		break;
	case 6: //列出该用户所有文件清单
		ListAllFile();
		break;
	case 7: //打开文件
		if((strcmp(buffer,".txt"))==0){
			cout<<"\n输入命令出错！";
			return;
		}
		OpenFile();
		break;
	case 8: //读文件
		if((strcmp(buffer,".txt"))==0){
			cout<<"\n输入命令出错！";
			return;
		}
		ReadFile();
		break;
	case 9: //写文件
		if((strcmp(buffer,".txt"))==0){
			cout<<"\n输入命令出错！";
			return;
		}
		WriteFile();
		break;
	default:
		break;
	}
}
void Command()//读取用户输入的命令，并将其转换成系统能识别的命令
{
	int len = 0,i,j;
	int cmdset;
	while(1)
	{
		cmdset = 0;
		cout<<"\n执行2";
		cin>>cmd;
		len = strlen(cmd);
		i=0;j=0;
		while(cmd[i]=='>'||cmd[i]==' '){i++;}//过滤空格键和'>'
		for(;i<len;i++)
		{
			
			if(cmd[i]=='>' || cmd[i]==' ' || i==len-1)
			{
				if(cmd[i]=='>' || cmd[i]==' ')
					buffer[j] = '\0';
				else
					if(i==len-1)
					{
		    			buffer[j]=cmd[i];
		    			buffer[j+1]='\0';
					}
				i++;
				j=0;
				int low=1,mid,high=keynum-1;
				bool BOOL = false;
        	    while(low<=high){//找到该命令关键字的内部识别码
					mid=(low+high)/2;
					if (strcmp(buffer,KWORD[mid])<=0) high=mid-1;
				   	if (strcmp(buffer,KWORD[mid])>=0) low=mid+1;		
					if(strcmp(buffer,KWORD[mid])==0){ 
				       		BOOL = true;
				       		break;
					}
				}
				if(!BOOL)
				{
					cout<<"\n"<<buffer<<"不是系统定义的命令...";

					cmdset = 0; break;
				}
				else {cmdset = mid;break;}
			}
			else{
				buffer[j] = cmd[i];
		    	j++;
			}
		}
		if(cmdset == 0) continue;
		while(cmd[i]=='>'||cmd[i]==' '){i++;}//过滤空格键和'>'
		buffer[0]='\0';
		Execute(i,len,cmdset);  //执行该命令
	}
}

int main()
{
	initscr();
	while(1){
    	int SELETE = LoginDisplay();
		if(SELETE==0)
			exit(0);
    	bool BOOL = Login(SELETE);//用户登陆，或者注册函数
    	if(BOOL)
		{
	    	KeyWord();  //初始化命令关键字
	    	DisplayUFD();//打印用户目录下的文件
	        Command();   //命令行操作
		}
	}
	endwin();
	return 0;
}