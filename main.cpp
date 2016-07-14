#include <iostream>
#include <fstream>
#include <string>
using namespace std;
void replace(int index,char buf[],int line,int col,char new_data[][32]){
	int i = index;
	int len_old = 0;
	while(buf[i] != ','){
		i++;
		len_old++;
	}
	i = 0;
	if(col == 0){
		int len_new = 0;
		while(new_data[line][i] != 0x20){
			len_new ++;
			i++;
		}
		i = 0;
		for(int j = 0; j < len_new; i++,j++)
			buf[i+index] = new_data[line][j];
		if(len_new < len_old){
			buf[i+index] = ',';
			i++;
			while(buf[i+index] != 0x20){
				buf[i+index] = 0x20;
				i++;
			}
		}
	}else{
		int len_new = 0;
		int k = 0;
		for(int j = 0; j < strlen(new_data[line]); j++,len_new++){
			if(new_data[line][j] == '\t'){
				len_new = -2;
				k = j + 1;
			}
		}
		i = 0;
		for(int j = 0; j < len_new; i++,j++)
			buf[i+index] = new_data[line][k++];
		if(len_new < len_old){
			buf[i+index] = ',';
			i++;
			while(buf[i+index] != 0x20){
				buf[i+index] = 0x20;
				i++;
			}
		}
	}
}
void modify_txt(){
	string filename;
	printf("请输入要修改的文件名(包含具体的路径，如D://test.txt):\n");
	cin>>filename;
	ifstream in_old(filename.c_str());
	if(! in_old.is_open()){
		printf("打开文件失败，请检查文件名\n");
		return;
	}
	printf("请输入新的数据文件路径\n");
	cin>>filename;
	ifstream in_new(filename.c_str());
	if(! in_new.is_open()){
		printf("打开文件失败，请检查文件名\n");
		return;
	}
	printf("请指定修改后的新文件的输出路径（请不要使用已存在的文件路径！否则会覆盖原文件！）\n");
	cin>>filename;
	ofstream out(filename.c_str());

	char new_data[100][32];
	in_new.getline(new_data[0],32);
	int line = 0;
	while(!in_new.eof()){
		in_new.getline(new_data[line++],32);
	}
	char buf[128];
	line = 0;
	int t = 0;
	while(!in_old.eof()){
		in_old.getline(buf, 128);
		t++;
		int col = 1;
		int flag = 0;
		if(t > 3){
		for(int i = 0; i < strlen(buf); i++){
			if(flag == 0 && col == 9 && buf[i] != 0x20){
				replace(i,buf,line,0,new_data);
				flag = 1;
			}
			if(flag == 1 && col == 10 && buf[i] != 0x20){
				replace(i,buf,line,1,new_data);
				line++;
				break;
			}
			if(buf[i] == ',') col++;
		}
		}
		out<<buf<<endl;
	}
	in_old.close();
	in_new.close();
}

int main(){
	modify_txt();
	return 0;
}