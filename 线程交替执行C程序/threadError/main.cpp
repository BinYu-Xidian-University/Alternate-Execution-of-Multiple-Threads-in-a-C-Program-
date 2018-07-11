#include <afx.h>
#include <stdio.h>
#include <process.h>
#include <afxwin.h>
int turn;
CWinThread* manager, *childHandle1,*childHandle2;
FILE *fp1;

unsigned int ChildThread1(LPVOID para){
	while (true){
		fprintf(fp1, "a");
		turn = 0;
		while (turn != 1){
			SwitchToThread();
		}
	}
}
unsigned int ChildThread2(LPVOID para){
	while (true){
		fprintf(fp1, "b");
		turn = 0;
		while (turn != 1){
			SwitchToThread();
		}
	}
}

unsigned int ManagerThread(LPVOID para)
{
	fp1 = fopen("result.txt", "w+");
	int i = 0;
	while (i<10000){
		turn = 1;
		childHandle1->ResumeThread();
		while (turn != 0){
			SwitchToThread();
		}
		childHandle1->SuspendThread();
		turn = 1;
		childHandle2->ResumeThread();
		while (turn != 0){
			SwitchToThread();
		}
		childHandle2->SuspendThread();
		i++;
	}
	fclose(fp1);
	return 1;
}


int main()
{
	manager = new CWinThread(ManagerThread, NULL);
	manager->CreateThread(CREATE_SUSPENDED);
	childHandle1 = new CWinThread(ChildThread1, NULL);
	childHandle1->CreateThread(CREATE_SUSPENDED);
	childHandle2 = new CWinThread(ChildThread2, NULL);
	childHandle2->CreateThread(CREATE_SUSPENDED);
	if (manager == NULL || childHandle1 == NULL || childHandle2 == NULL){
		return 1;
	}
	manager->ResumeThread();
	WaitForSingleObject(manager->m_hThread, INFINITE);
}