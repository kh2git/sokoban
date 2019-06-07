#include <stdio.h>
#include <stdlib.h>
#include <termio.h>

char name[10]; //이름저장소
int mvcnt = 0; //움직임카운트
int stage = 1; //스테이지 값
char map[5][30][30] = {0};//맵
int x_check, y_check; //창고지기의 위치
int score[5][5] = {0};	// 스테이지, 순위별 이동횟수 저장소
int rsc[5] = {0};	// 여기에 방금 끝낸 게임 이동횟수 들어감
char names[5][5][10];	// 각 스테이지/순위별 플레이어 이름 저장소

int getch(void)
{
	int ch;

	struct termios buf;
	struct termios save;

	tcgetattr(0, &save);
	buf = save;

	buf.c_lflag &= ~(ICANON | ECHO);
	buf.c_cc[VMIN] = 1;
	buf.c_cc[VTIME] = 0;

	tcsetattr(0, TCSAFLUSH, &buf);

	ch = getchar();
	tcsetattr(0, TCSAFLUSH, &save);

	return ch;
}


   void map_open() //맵 입력
{

   FILE*fp;

   fp = fopen("map.txt","r");

   char t=0;

   for(int z=0; z<5; z++){

    for(int y=0; y<30; y++){

            for(int x=0; x<30; x++){
                fscanf(fp,"%c",&t);
                if(t=='\n')
                break;
                if(t=='2'||t=='3'||t=='4'||t=='5'||t=='e'){
                break;
                }
                else
                    map[z][x][y]=t;
            }
            if(t=='2'||t=='3'||t=='4'||t=='5'||t=='e'){
                break;
                }
        }
            }
   fclose(fp);
    }




void map_cnt() //맵박스개수체크
{
   int cnt1 = 0;
   int cnt2 = 0;
   for (int i = 0; i < 5; i++) {
	  for (int j = 0; j < 30; i++) {
		 for (int k = 0; k < 30; k++) {
			if (map[i][j][k] == '$')
			   cnt1++;
			if (map[i][j][k] == 'O')
			   cnt2++;
		 }
	  }
	  if (cnt1 != cnt2) {

	  printf("잘못된 맵입니다.");

	  }
	  cnt1 = 0;
	  cnt2 = 0;
   }

}


void inputname() //이름입력
{
   printf("input name : ");
   scanf("%s",name);
}

void check() //창고지기의 위치
{
	int x, y;
	for(x=0; x<30; x++){
		for(y=0; y<30; y++){
			if(map[stage-1][x][y]=='@'){
				x_check = x;
				y_check = y;
		}
	}
}
	
void map_print() //맵출력
{
	system("clear");
	for(int i=0; i<10; i++){
  	printf("%c",name[i]);
   	}
	printf("\n");
	for (int i = 0; i < 30; i++) {
		for (int j = 0; j < 30; j++) {
			printf("%c",map[stage-1][j][i]);
		}
		printf("\n");
	}
}

void inputkey(char input_char) //키보드입력
{
   switch (input_char)
   case 'h':
	x = -1
	cnt++;
	break;
   case 'j':
	y = -1
	cnt++;
	break;
   case 'k':
	y = 1;
	cnt++;
	break;
   case 'l':
	x = 1;
	cnt++;
	break;
   case 'u':
	cnt++;
	break;
   case 't':
	
	break;
   case 'n':
	
	break;
   case 'e':
	
	break;
   case 's':
	
	break;
   case 'f':
	
	break;
   case 'd':
	
	break;
}


void man() //d누르면 실행될 수 있도록 만들어야함//
{
   system("clear")
   printf("창고지기 조정 : 왼쪽-h, 아래-j, 위-k, 오른쪽-l\n");
   printf("가장 최근의 이동기록 취소(5번까지 사용가능 - u\n");
   printf("현재 맵을 처음부터 다시시작 - r\n");
   printf("첫번째 맵부터 다시시작 - n\n");
   printf("게임 종료(자동저장) - e\n");
   printf("현재 상태 파일에 저장 - s\n");
   printf("save 지점부터 이어서 게임진행 - f\n");
   printf("게임 순위 출력 - t\n");
}

void con()//단계별 완료시 축하메시지
{
   printf("Congratulation");
   return 0;
}

void ranking()	//랭킹입력함수
{
	FILE*ifp;
	FILE*ofp;
	char rdata;
	ifp = fopen("ranking.txt", "r"); 
	ofp = fopen("ranking.txt", "w");
	fscanf(ipf,"%c",&rdata);
	if (rdata =! 'm')
	{
		for (int i=0; i<=4; i++)
		{
			int n = i + 1;
			fprintf(ofp, "map%d\n\n",n);
			for (int k=0; k<=9; k++)
				name[i][0][k] = name[l];
			for (int j=0; j<=4; j++){
				for (int k=0; k<=9; k++)
					fprintf(ofp, "%c", names[i][j][k]);
				fprintf(ofp, " %d\n\n", score[i][j]);
			}
		}
	}
	else
	{
		for (int i=0; i<=4; i++)
		{
			int n = i + 1;
			fprintf(ofp, "map%d\n\n",n);
			for (int j=0; j<=4; j++)
			{
				if (rsc[i] < score[i][j]){	//이동횟수 비교
					score[i][j] = rsc[i];
					names[i][j] = name;
				}
				else if (rsc[i] = score[i][j]) || j<4){
					score[i][j+1] = rsc[i];
					names[i][j+1] = name;
				}
				else
					;
				for (int j=0; j<=4; j++){
					for (int k=0; k<=9; k++)
						fprintf(ofp, "%c", names[i][j][k]);
				fprintf(ofp, " %d\n\n", score[i][j]);
				}
			}
		}
	}
}

int main(void)
{
	char a;
	printf("Start...");
	inputname();
	system("clear");
	map_open();
	while(1){
	printf("Hello %s\n",name);
	map_print();
	a=getch();
	void inputkey(a);
	}
}
