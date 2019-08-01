// StarPopRPG.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//星星消消乐


#include "pch.h"
#include <iostream>
#include <vector>
#include <ctime>
#define N 100
using namespace std;

class ChessManual
{
public:
	static ChessManual* getChessManual();
	
	void randFullChess(int const starNumber);
	void pChessArray();
	bool selectChessSetArray(int x, int y, int *array = NULL);	//array参数为了 detectionNotChess重用，为null时修改源棋牌，否则返回是否有可以消的方块
	void searchChessArray(int x, int y, int need,vector<int> *chessIndex); // 回溯算法检测相连
	bool isVectorElem(vector<int> ve,int need);
	void trimChessArrayY(int x);  //棋牌下移
	void trimCheesArrayX(); //棋牌左移
	bool detectionNotChess();//检测棋牌中是否还有可消方块
	bool setNowGrade(int popNumber);
	void nextPass();

	int getNowGrade();
	int getPassNeedGrade();
	int getPass();
private:
	ChessManual();
	~ChessManual();

	static ChessManual* m_ChessManual;
	int m_starNumber;
	int m_ChessArray[N];
	int m_InitNullRow = 0; //pass init
	int m_NowGrade = 0;

	int m_Pass = 1;
	int m_PassNeedGrade = 1000;
};

ChessManual* ChessManual::m_ChessManual = NULL;

ChessManual * ChessManual::getChessManual()
{
	if (m_ChessManual == NULL)
	{
		
		m_ChessManual = new ChessManual();
	}
	return m_ChessManual;
}

void ChessManual::pChessArray()
{
	system("cls");
	int row = 0;
	cout << "   0  1  2  3  4  5  6  7  8  9 " << endl;
	for (size_t i = 0; i < N; i++)
	{
		if (i == 0)
		{
			cout << row << " ";
			row++;

		}
		if (m_ChessArray[i] == -1)
		{
			cout << "[ ]";
		}
		else
		{
			for (size_t j = 0; j < m_starNumber; j++)
			{
				if (m_ChessArray[i] == j)
				{
					cout << "[" << j << "]";
				}

			}
		}
		

		

		if (i % 10 == 9)
		{
			cout << endl;
			if (row == 10)
			{

			}
			else
			{
				cout << row << " ";
				row++;
			}

		}
	}
}

bool ChessManual::selectChessSetArray(int x, int y,int *array)
{
	int need;
	if (array == NULL)
	{
		 need= m_ChessArray[y * 10 + x];
	}
	else
	{
		need = array[y * 10 + x];
	}

	if (need != -1)
	{
		vector<int> chessIndex;
		searchChessArray(x, y, need, &chessIndex);
		vector<int>::iterator iter;
		int chessIndexN = 0;
		for (iter = chessIndex.begin(); iter != chessIndex.end(); iter++) {
			chessIndexN++;
		}
		if (chessIndexN > 1)
		{
			if (array == NULL)
			{
				setNowGrade(chessIndexN);
				for (iter = chessIndex.begin(); iter != chessIndex.end(); iter++) {
					m_ChessArray[*iter] = -1;
				}
				for (size_t x = 0; x < 10; x++)
				{
					trimChessArrayY(x);
				}
				trimCheesArrayX();
			}
			
			return true;
		}
	}
	return false;
	
	
}

void ChessManual::searchChessArray(int x, int y, int need, vector<int> *chessIndex)
{
	if (m_ChessArray[y * 10 +x] == need)
	{
		chessIndex->push_back(y * 10 + x);
		if (y-1>-1 && !isVectorElem(*chessIndex, (y - 1) * 10 + x) && m_ChessArray[(y-1)*10 +x] == need )
		{
			searchChessArray(x, y - 1, need,chessIndex);
		}
		if (x + 1<10 && !isVectorElem(*chessIndex,y * 10 + (x + 1)) && m_ChessArray[y * 10 + (x+1)] == need)
		{
			searchChessArray(x + 1, y, need, chessIndex);
		}
		if (y + 1 < 10 && !isVectorElem(*chessIndex,(y + 1) * 10 + x) && m_ChessArray[(y + 1) * 10 + x] == need)
		{
			searchChessArray(x, y + 1, need, chessIndex);
		}
		if (x-1 > -1 && !isVectorElem(*chessIndex, y * 10 + (x - 1)) && m_ChessArray[y * 10 + (x-1)] == need)
		{
			searchChessArray(x - 1, y, need, chessIndex);
		}
	}
}

bool ChessManual::isVectorElem(vector<int> ve,int need)
{
	vector<int>::const_iterator iter;
	for (iter = ve.begin(); iter != ve.end(); iter++)
	{
		if (*iter == need)
		{
			return true;
		}
	}
	return false;
}

void ChessManual::trimChessArrayY(int x)
{
	vector<int> needArray;
	for (int y = 0; y < 10; y++)
	{
		if (m_ChessArray[y * 10 + x] != -1)
		{

			needArray.push_back(m_ChessArray[y * 10 + x]);

		}
		
	}
	for (int a = 9; a > -1; a--)
	{
		if (!needArray.empty())
		{
			m_ChessArray[a * 10 + x] = needArray.back();
			needArray.pop_back();
		}
		else
		{
			m_ChessArray[a * 10 + x] = -1;
		}
		
	}


}

void ChessManual::trimCheesArrayX()
{
	int index = -1;
	for (int i = 0; i < 10 - m_InitNullRow; i++)
	{
		if (m_ChessArray[90+i] == -1)
		{
			
			index = i;
			break;
		}
	}
	if (index != -1)
	{
		for (int i = index; i < 10-m_InitNullRow; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				if (i == 10 - m_InitNullRow - 1)
				{
					m_ChessArray[j * 10 + i] = -1;
					
				}
				else
				{
					m_ChessArray[j * 10 +  i] = m_ChessArray[j * 10 + 1 + i];

				}
			}
		}
		m_InitNullRow++;
		trimCheesArrayX();
	}

}

bool ChessManual::detectionNotChess()
{
	if (m_InitNullRow > 5)
	{
		int copyChessArray[100];
		memcpy(copyChessArray, m_ChessArray, sizeof(m_ChessArray));
		for (int x = 0; x < m_InitNullRow; x++)
		{
			for (int y = 9; y > -1; y--)
			{
				if (selectChessSetArray(x,y,copyChessArray))
				{
					return false;
				}
				
			}
		}
		return true;
		
	}
	return false;
}

bool ChessManual::setNowGrade(int popNumber)
{
	if (popNumber < 6)
	{
		m_NowGrade += popNumber * 10 + (popNumber - 2) * 10;
		return true;
	}
	if (popNumber < 10)
	{
		m_NowGrade += popNumber * 10 + (popNumber - 2) * 10 + (popNumber - 5) * 10;
		return true;
	}
	else
	{
		m_NowGrade += popNumber * 10 + (popNumber - 2) * 10 + (popNumber - 5) * 10 + 10;
		return true;
	}
}

void ChessManual::nextPass()
{
	m_Pass++;
	m_InitNullRow = 0;
	m_PassNeedGrade = 1000 + m_Pass * 1000 ;
	randFullChess(m_starNumber);

}

int ChessManual::getNowGrade()
{
	return m_NowGrade;
}

int ChessManual::getPassNeedGrade()
{
	return m_PassNeedGrade;
}

int ChessManual::getPass()
{
	return m_Pass;
}

void ChessManual::randFullChess(int const starNumber)
{
	m_starNumber = starNumber;
	srand(static_cast<unsigned int>(time(0)));
	for (size_t i = 0; i < N; i++)
	{
		int q = rand() % m_starNumber;
		for (size_t j = 0; j < m_starNumber; j++)
		{
			if (q == j)
			{
				m_ChessArray[i] = j;
			}
		}
	}
}

ChessManual::ChessManual()
{
	memset(m_ChessArray, 0, sizeof(int) * 100);
}

ChessManual::~ChessManual()
{
}

int main()
{
	int x=0,y=0;

	ChessManual::getChessManual()->randFullChess(3);
	do
	{
		ChessManual::getChessManual()->pChessArray();//打印棋牌
		if (ChessManual::getChessManual()->detectionNotChess())
		{
			//检测是否死局
			if (ChessManual::getChessManual()->getNowGrade() > ChessManual::getChessManual()->getPassNeedGrade())
			{
				ChessManual::getChessManual()->nextPass();
				ChessManual::getChessManual()->pChessArray();
			}
			else
			{
				break;
			}
		

		}
		cout << endl;
		cout << "当前分数：" << ChessManual::getChessManual()->getNowGrade() << "    " << "当前关卡: ";
		cout << ChessManual::getChessManual()->getPass()<<"   "<<"需要分数: "<< ChessManual::getChessManual()->getPassNeedGrade()<<endl;
		cout << endl;
		do
		{
			cout << "请输入坐标：" << endl;
			cin >> x;
			cin >> y;
		} while (!(x < 10 && y < 10));
		
		
		ChessManual::getChessManual()->selectChessSetArray(x, y);
		cout << endl << endl;

	} while (true);
	

}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
