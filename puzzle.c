#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define empty ' '


/*初期化*/
void init1(char BG[12][6]){
  int y,x;
  for(y=0;y<12;y++){
    for(x=0;x<6;x++){
      BG[y][x]=empty;
    }
  }
}

void init2(char nx[3][3],char r1,char r2){
  nx[0][0] = empty;
  nx[0][1] = empty;
  nx[0][2] = empty;
  nx[1][0] = empty;
  nx[1][1] = r1;
  nx[1][2] = empty;
  nx[2][0] = empty;
  nx[2][1] = r2;
  nx[2][2] = empty;
}

/*色*/
void colorize(char a){
  switch(a){
    
  case empty:
    printf("%c",a);
    break;
    
  case 'a':
    printf("\033[31m");
    printf("%c",a);
    printf("\033[39m");
    break;

  case 'b':
    printf("\033[32m");
    printf("%c",a);
    printf("\033[39m");
    break;

  case 'c':
    printf("\033[33m");
    printf("%c",a);
    printf("\033[39m");
    break;

  case 'd':
    printf("\033[34m");
    printf("%c",a);
    printf("\033[39m");
    break;

  case 'e':
    printf("\033[35m");
    printf("%c",a);
    printf("\033[39m");
    break;
  }
}

/*背景の表示*/
void show(char BG[12][6],int d){
  printf("\033[2J");
  printf("score:%d\n\n",d);
  printf(" 0 1 2 3 4 5\n");
  int y,x;
  for(y=0;y<12;y++){
    for(x=0;x<6;x++){
      printf("|");
      colorize(BG[y][x]);
    }
    printf("|\n");
  }
}

/*ユーザに0から6の数字を入力を受け付ける*/
int input(){
  int i,r;
  while(1){
    printf("中央のブロックを落とす列を入力(0-5)\n回転は(6):");
    r = scanf("%d",&i);
    if(r!=1){
      printf("再入力\n");
      scanf("%*s");
      continue;
    }
    if(i<0||6<i){
      printf("再入力\n");
      continue;
    }
    break;
  }
  return(i);
}

/*文字を1つ生成*/
int charmaker(){
  /*0～4の乱数を発生*/
  int x;
  int x_max=4;
  int x_min=0;
  x = x_min + (int)(rand()*(x_max - x_min + 1.0)/(1.0 + RAND_MAX));

  /*乱数をa～eの文字に変換*/
  char str[6] ="abcde";
  return(str[x]);
}

/*回転（実際に回転はしてないない）*/
int rotate1(int x){
  if(0<=x&&x<6)
    return 0;
  return 1;
}

/*回転*/
void rotate2(char nx[3][3]){
  if(nx[2][1]!=empty){
    nx[1][2]=nx[2][1];
    nx[2][1]=empty;
  }else if(nx[1][2]!=empty){
    nx[0][1]=nx[1][2];
    nx[1][2]=empty;
  }else if(nx[0][1]!=empty){
    nx[1][0]=nx[0][1];
    nx[0][1]=empty;
  }else if(nx[1][0]!=empty){
    nx[2][1]=nx[1][0];
    nx[1][0]=empty;
  }
}

/*次のブロックのペアを表示させる*/
void next_indicator(char nx[3][3]){
  int n,m;
  printf("\nnext block:\n");
  for(n=0;n<3;n++){
    for(m=0;m<3;m++){
      colorize(nx[n][m]);}
    putchar('\n');
  }
}

/*ブロックを落とす(縦と横の向きで対応が異なる）*/
int drop(char BG[12][6],int x,char nx[3][3]){
  int n,m; 
  if(nx[2][1]!=empty){
    for(n=11;n>=0;n--){
      if(BG[n][x]==empty){
	BG[n][x]=nx[2][1];
	BG[n-1][x]=nx[1][1];
	break;
      }
    }  
  }
  else if(nx[1][2]!=empty){
    for(n=11;n>=0;n--){
      if(BG[n][x]==empty){
	BG[n][x]=nx[1][1];
	break;
      }
    }for(m=11;m>=0;m--){
      if(BG[m][x+1]==empty){
	BG[m][x+1]=nx[1][2];
	break;
      }
    }
  }
  else if(nx[0][1]!=empty){
    for(n=11;n>=0;n--){
      if(BG[n][x]==empty){
	BG[n][x]=nx[1][1];
	BG[n-1][x]=nx[0][1];
	break;
      }
    }   
  }
  else if(nx[1][0]!=empty){
    for(n=11;n>=0;n--){
      if(BG[n][x]==empty){
	BG[n][x]=nx[1][1];
	break;
      }
    }for(m=11;m>=0;m--){
      if(BG[m][x-1]==empty){
	BG[m][x-1]=nx[1][0];
	break;
      }
    }
  }   
  return 0;
}  

/*同じものの隣接を数える*/
void count(char f[12][6],int y,int x,int *n){
  char c=f[y][x]; 
  if(c!=empty){
    f[y][x]=0;
    *n=*n+1;
    if(((x+1)<6)&&(f[y][x+1]==c))
      count(f,y,x+1,n);
    if(((y+1)<12)&&(f[y+1][x]==c)) 
      count(f,y+1,x,n);
    if(((x-1)>=0)&&(f[y][x-1]==c)) 
      count(f,y,x-1,n);
    if(((y-1)>=0)&&(f[y-1][x]==c)) 
      count(f,y-1,x,n);
    f[y][x]=c;
  }
}

/*状態をコピーする*/
void copy(char to[12][6],char from[12][6]){
int y,x;
  for(y=0;y<12;y++){
    for(x=0;x<6;x++){
      to[y][x]=from[y][x];
    }
  }
}

/*ぷよを消すpart1(準備関数）*/
void vanish(char f[12][6],int y,int x){
  char c=f[y][x];
    f[y][x]=empty; // ぷよを消す
    if(((x+1)<6)&&(f[y][x+1]==c))
      vanish(f,y,x+1);
    if(((y+1)<12)&&(f[y+1][x]==c))
      vanish(f,y+1,x);
    if(((x-1)>=0)&&(f[y][x-1]==c))
      vanish(f,y,x-1);
    if(((y-1)>=0)&&(f[y-1][x]==c))
      vanish(f,y-1,x);
}

/*ぷよを消すpart2*/ 
int delete(char BG[12][6]){
  int x,y,n;
  char f[12][6];
  char c;
  copy(f,BG);
  for(y=0;y<12;y++){
    for(x=0;x<6;x++){
      c=f[y][x];
      if(c!=empty){
	n=0;
	count(f,y,x,&n);
	if(n>=4){
	  vanish(f,y,x); 
	  copy(BG,f);
	  return n;
	}
      }
    }
  }
  return 0;
}

/*ぷよを落下させる（連鎖準備）*/
void fall(char BG[12][6]){
  int x,y,j,i;
  for(i=0;i<30;i++){
    for(x=0;x<6;x++){
      for(y=11;y>=0;y--){
	if(BG[y][x]==empty){
	  for(j=y-1;j>0&&BG[j][x]==empty;j--);
	  if(j<0)
	    break;
	  for(j=y;j>=0;j--){
	    if(j-1>=0){
	      BG[j][x]=BG[j-1][x];
	    }else 
	      BG[j][x]=empty;
	  }
	}
      }
    }
  }
}

/*gameover判断*/
int gameover(char BG[12][6]){
  int k,l=0;
  for(k=0;k<6;k++){
    if(BG[0][k]!=empty)
      l++;
  }
  if(l==0)
    return 0;
  else
    return 1;
}

int main(){  
  /*乱数を初期化*/
  srand(time(NULL));
  int sum=0;
  char BG[12][6];
  init1(BG);
  char f[12][6];
  init1(f);
  char nx[3][3];
  int k;
  
  /*ここからゲームのループ*/
  while(1){
    char r1=charmaker();
    char r2=charmaker();
    init2(nx,r1,r2);
    while(1){
      show(BG,sum);
      next_indicator(nx);
      int i=input();
      if(rotate1(i)){	
	rotate2(nx);
	printf("\033[2J");
      }else{
	drop(BG,i,nx);
	sum=sum+delete(BG);
	for(k=2;k<20;k++){
	  fall(BG);
	  sum=sum+((delete(BG))*k);
	}
	if(gameover(BG)){
	  printf("GAMEOVER");
	  return 0;
	}
	printf("\033[2J");
	break;
      }
    }
  }
  return 0;
}

