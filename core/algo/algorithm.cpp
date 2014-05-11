#include "Normalization.h"
#include <math.h>

void Iteration::preparing()
{

	myint keyNodeCounter=0;

	//поиск нужных строк  В МАТРИЦЕ СМЕЖЕННОСТИ ключевые вершины равно 1, остальные 0
	// ЗАПИСЬ ЗНАЧЕНИЙ НАЙДЕННЫХ СТОЛБЦОВ В ХЕД, А КОЛИЧЕСТВА НАХОЖДЕНИЙ В кейнодкаунтер

	//Вычисление кейнода
	for (int i=1; i<mainM_x; i++) {         
		for (int j=1; j<mainM_y; j++)	{
            if ((*pMainM)[i][j]==1)	{
				break;
			}
			if (j==(mainM_y-1)&&((*pMainM)[i][j]==0))	{
				keyNodeCounter++;
			}

		}
	}

    //
    try	{
    if (keyNodeCounter==0)
        throw 101;
    }
    catch (...)	{
        cout << "В матрице смеженности не обнаруженно ключевых полей" << "\n";
        //system("pause");
        //exit;
    }
	//---------------------------------------------
	// Объявление и инициализация ХЕДЕРА

	headerM_x=keyNodeCounter;
	headerM=new myint [headerM_x];
	
	for (int i=1, k=0; i<mainM_x; i++) {         
		for (int j=1; j<mainM_y; j++)	{
			if ((*pMainM)[i][j]==1)	{
				break;
			}
			if (j==(mainM_y-1)&&((*pMainM)[i][j]==0))	{
				headerM[k]=i-1;
				k++;
			}

		}
	}

	//Обрезка ключевых строк из mainM -> mainMcr
	mainMcr_x=mainM_x-keyNodeCounter;
    pMainMcr= new Matrix(mainMcr_x, mainM_y);


	//Копирование из mainM -> mainMcr с поправкой 
	//НУЖНО УБРАТЬ В МЕТОД КЛАССА МАТРИКС
	//TODO:решить относительное содерж хедера или реальное
	for (int j=0; j<mainM_y; j++)
		(*pMainMcr)[0][j]=(*pMainM)[0][j];
    for (int i=1, mod=0; i<mainM_x; i++)	{
        if (i==(headerM[mod]+1) && mod < headerM_x)	{
			mod++;
		}
		else    {
			for (int j=0; j<mainM_y; j++)	{
				(*pMainMcr)[i-mod][j]=(*pMainM)[i][j];
			}	
		}
	

	}

	cout << "Метод Препэрин";
	cout << "\n";
	cout << "MainMcr";
	cout << "\n";
	pMainMcr -> show();

	//---------------------------------------------------------------------------------------------------------
	//--------Создание матрицы наборов-------
	//---------Динамическая матрица наборов создание

	setsM_x=(myint)pow(2, keyNodeCounter);
	setsM_y=keyNodeCounter;
	
	pSetsM= new Matrix(setsM_x, setsM_y);     


	//Инициализация матрицы наборов
	//// Функция заполнения матрицы наборов
	//------------------------------------заполняем матрицу единицами
	//----------------------------------заполяем последовательностью нулей и единиц

	pSetsM -> fillBinarySequence();

	cout << "HeaderM";
	cout << "\n";
	showHeaderM();

	cout << "Sets";
	cout << "\n";
	(*pSetsM).show();
	//-------------------------------------------------------------
	//Создание матрицы 2НФ
	//------------------------------------------------------------

	secondM_x=mainMcr_x;
	secondM_y=setsM_x+1;

	pSecondM= new Matrix (secondM_x, secondM_y);     
	

	pSecondM -> fillWithNumber(8);
	//----------------------------------------
	// Цикл заполнения матрицы 2НФ на основе матрцы наборов, матрицы смеженности и всмомогательной матрицы headerM                                                            //Зубодробящая логика цикла
	//----------------------------------------




	pSecondM -> addIndexRowCol(pMainMcr->getPtr());










	for (int i=0; i<setsM_x;i++)	{
		myint setN=0;
		for (int j=0; j<setsM_y;j++)	{
			setN+=(*pSetsM)[i][j]*(myint)pow(10,j);
			
		}
		//cout << setN << " ";
		//cout << "\n";
		for (int k=1; k<mainMcr_x; k++)	{
			myint mainN=0;
					
			for (int l=0; l<headerM_x;l++)	{
				mainN+=(*pMainMcr)[k][ headerM[l]+1 ]*(myint)pow(10,l);

			}
			if (mainN==setN)	
				(*pSecondM)[k][i+1]=1;
			else
				(*pSecondM)[k][i+1]=0;
			
		}


	}

	cout << "SecondM";
	cout << "\n";
	pSecondM -> show();

}

void Iteration::secondNF()
{
	int colAmount=1;

	// буферный массив с обозначением нужных столбцов и количество нужных столбцов
	myint *goodCol= new myint [secondM_y]; 
	goodCol[0]=1;
	//поиск нужных столбцов
	for (int j=1; j<secondM_y; j++) {         
		for (int i=1; i<secondM_x; i++) 
			if ((*pSecondM)[i][j]==1) 
				{goodCol[j]=1; ++colAmount; break;}
			else 
				{goodCol[j]=0;}

		}

	//объявление динамического массива 2
	secondM2_x=secondM_x;
	secondM2_y=colAmount;
	pSecondM2=new Matrix(secondM2_x, secondM2_y);     
	
	//заполнение массива 2 из массива 1 согласно информации из вспомагательного массива                            
	for (int j=0, mod=0; j<secondM_y; j++) {
	if (goodCol[j]==1) {
		for (int i=0; i<secondM_x; i++) (*pSecondM2)[i][j-mod]=(*pSecondM)[i][j];
		}
	else ++mod;
	}


	cout << "Метод Секонд НФ";
	cout << "\n";
	cout << "SecondM2";
	cout << "\n";
	pSecondM2 -> show();
















	delete [] goodCol;
}

void Iteration::thirdNF()
{


	//
	//УДАЛЕНИЕ СТРОК С КЛЮЧЕВЫМИ ВЕРШИНАМИ
	//
	myint thirdMtmp_x=mainM_x-headerM_x;
	Matrix thirdMtmp(thirdMtmp_x, mainM_y);
	
	//Копирование  с поправкой
	for (int j=0; j<mainM_y; j++)	
		thirdMtmp[0][j]=(*pMainM)[0][j];
	for (int i=1, mod=0; i<mainM_x; i++)	{
        if (i==(headerM[mod]+1) && mod < headerM_x)	{
			mod++;
		}
		else
		{
			for (int j=0; j<mainM_y; j++)	{
				thirdMtmp[i-mod][j]=(*pMainM)[i][j];
			}				
		}

	}


	//вывод на экран

	cout << "Метод ФёрдНФ";
	cout << "\n";
	cout << "thirdMtmp";
	cout << "\n";
	thirdMtmp.show();


	//УДАЛЕНИЕ СТОЛБЦОВ С КЛЮЧЕВЫМИ ВЕРШИНАМИ thirdMtmp_x * thirdMtmp2_y

	myint thirdMtmp2_y=mainM_y-headerM_x;

	//Копирование  с поправкой
	for (int j=0; j<thirdMtmp_x; j++)	
		thirdMtmp[j][0]=thirdMtmp[j][0];
	for (int i=1, mod=0; i<mainM_y; i++)	{
        if (i==(headerM[mod]+1)&& mod < headerM_x)	{
			mod++;
		}
		else
		{
			for (int j=0; j<thirdMtmp_x; j++)	{
				thirdMtmp[j][i-mod]=thirdMtmp[j][i];
			}		
		}
	}

	//Ответвление вычислений с помощью матрицы аутМтмп
	Matrix outMtmp(thirdMtmp_x, thirdMtmp_x);
	for (int i=0; i<thirdMtmp_x;i++)	
		for (int j=0;j<thirdMtmp2_y; j++)
			outMtmp[i][j]=thirdMtmp[i][j];

	//вывод на экран
	cout << "\n";

	for (int i=0; i<thirdMtmp_x;i++)	{
		for (int j=0;j<thirdMtmp2_y; j++)
			cout << thirdMtmp[i][j] << " ";
		cout << "\n";
	}
	
	cout << "\n";

	//УДАЛЕНИЕ НУЛЕВЫХ СТРОК

	//НАХОЖДЕНИЕ РАЗМЕРНОСТИ ПО Х БУДУЩЕГО МАССИВА

	myint thirdMtmp3Mod_x=0;
	for (int i=1; i<thirdMtmp_x; i++)
		for (int j=1; j<thirdMtmp2_y; j++)	{
			if (thirdMtmp[i][j]==1)	break;
			if (thirdMtmp[i][j]==0 && j==thirdMtmp2_y-1) thirdMtmp3Mod_x++;

		}

	//САМ СОБСТВЕННО ЦИКЛ УДАЛЕНИЯ НУЛЕВЫХ СТРОК MАССИВ thirdMtmp3_x*thirdMtmp2_y
	
	//cout << thirdMtmp3Mod_x;

	myint thirdMtmp3_x=thirdMtmp_x-thirdMtmp3Mod_x;

	try	{
		arrayIsNull=0;
		if (thirdMtmp3_x<2)	{
			arrayIsNull=1;
			throw 100;
		}

	


		for (int i=1, mod=0; i<thirdMtmp_x; i++)	{
			int sum1=0;
			for (int j=1; j<thirdMtmp2_y; j++)	{
				sum1=sum1+thirdMtmp[i][j];
			}
			if (sum1>0)	{
				for (int j2=0; j2<thirdMtmp2_y; j2++)	{
				thirdMtmp[i-mod][j2]=thirdMtmp[i][j2];
				}
			}
			else mod++;
			sum1=0;
		}


		//вывод на экран thirdMtmp3_x*thirdMtmp2_y
		cout << "\n";

		for (int i=0; i<thirdMtmp3_x;i++)	{
			for (int j=0;j<thirdMtmp2_y; j++)
				cout << thirdMtmp[i][j] << " ";
			cout << "\n";
		}
	
		cout << "\n";

		//УДАЛЕНИЕ НУЛЕВЫХ СТОЛБЦОВ
		//Нахождение размерности Y

		myint thirdMtmp4Mod_y=0;
		for (int j=1; j<thirdMtmp2_y; j++)
			for (int i=1; i<thirdMtmp3_x; i++)	{
				if (thirdMtmp[i][j]==1)	break;
				if (thirdMtmp[i][j]==0 && i==thirdMtmp3_x-1) thirdMtmp4Mod_y++;
			}

		//ЦИКЛ УДАЛЕНИЯ НУЛЕВЫХ СТОЛБЦОВ thirdMtmp3_x * thirdMtmp4_y

		myint thirdMtmp4_y=thirdMtmp2_y-thirdMtmp4Mod_y;

		for (int j=1, mod=0; j<thirdMtmp2_y; j++)	{
			int sum1=0;
			for (int i=1; i<thirdMtmp3_x; i++)	{
				sum1=sum1+thirdMtmp[i][j];
			}
			if (sum1>0)	{
				for (int i2=0; i2<thirdMtmp3_x; i2++)	{
				thirdMtmp[i2][j-mod]=thirdMtmp[i2][j];
				}
			}
			else mod++;
			sum1=0;
		}


		//удаление массива thirdMtmp и создание нового соглансо размерности thirdMtmp3_x * thirdMtmp4_y
		thirdM_x=thirdMtmp3_x;
		thirdM_y=thirdMtmp4_y;

		pThirdM= new Matrix(thirdM_x, thirdM_y);

		for (int i=0; i<thirdMtmp3_x;i++)	
			for (int j=0;j<thirdMtmp4_y; j++)
				(*pThirdM)[i][j]=thirdMtmp[i][j];



		//вывод на экран РЕЗУЛЬТИРУЕЮЩЕГО thirdM
		cout << "\n";
		cout << "ThirdM";
		cout << "\n";
		pThirdM -> show();
	
		//////////////////////////////////////////
		//////////////////////////////////////////////////////////////////
		// Поиск по Х и У для нахождения начальной размерности хедераН
		myint outMtmp2_x=outMtmp.getX();
		myint Nullrc=0;
        for (int m=1; m<outMtmp2_x; m++)	{
			myint sum=0;
			for (int n=1; n<outMtmp2_x; n++)	{
				sum=sum+outMtmp[m][n]+outMtmp[n][m];
			}
			if (sum > 0)	{
				Nullrc++;			
			}

		}
		// Объявление хедераН
		Matrix headerN(Nullrc,1);
		
		// Тоже самое но с заполнением хедераН и матрицы по Х со сдвигом
		for (int m=1, mod=0, headMod=0; m<outMtmp2_x; m++)	{
			myint sum=0;
			for (int n=1; n<outMtmp2_x; n++)	{
				sum=sum+outMtmp[m][n]+outMtmp[n][m];
			}
			if (sum==0)	{
	
				mod++;
			}
			if (sum > 0)	{
				headerN[headMod][0]=m;
				headMod++;
				for (int i=0; i<outMtmp2_x; i++)	{
					outMtmp[m-mod][i]=outMtmp[m][i];
				}		
			}

		}

		//Размерность новой матрицы по Х и У через нуллРЦ
		myint outMtmp3_xy=Nullrc+1;

		//Заполнение матрицы по У со сдвигом 
		for (int n=1, mod=0, headerMod=0; n<outMtmp2_x && headerMod<Nullrc; n++)	{
			if (n==(headerN[headerMod][0]))	{
				for (int m=0; m<outMtmp3_xy; m++)	{
					outMtmp[m][n-mod]=outMtmp[m][n];			
				}
				headerMod++;
			}
			else
			{
				mod++;
			}

		}

		// Заливка всего хозяйства в результирующую матрицу с правильной размерностью
		pOutM= new Matrix(outMtmp3_xy, outMtmp3_xy);
		for (int i=0; i<outMtmp3_xy;i++)	
			for (int j=0;j<outMtmp3_xy; j++)
				(*pOutM)[i][j]=outMtmp[i][j];

		//Вывод на экран
		pOutM -> show();
	}

	catch (...){
		cout << "/////////////////////////////////////////////////" << "\n";
		cout << "/////////////////////////////////////////////////" << "\n";
		cout << "//Нулевой массив остаточной матрицы смеженности//" << "\n";
		cout << "/////////////////////////////////////////////////" << "\n";
		cout << "/////////////////////////////////////////////////" << "\n";
		cout << "\n";
        pThirdM = 0; // nullptr
	}



}


void Iteration::construction()
{


	myint secondM3_x; 
	if (arrayIsNull)	secondM3_x=secondM2_x;
	else  secondM3_x=secondM2_x-thirdM_x+1; // +1 так как отнимается индекс столбец

	myint secondM3_y=secondM2_y;
	Matrix secondM3(secondM3_x, secondM3_y);

	//
	//УДАЛЕНИЕ СТРОК В СООТВЕТСТВИИ С НУЛЕВЫМ СТОЛБЦОМ ФЁРДА
	//
	//Копирование  с поправкой

	if (arrayIsNull)	{
		for (int i = 0; i<secondM2_x; i++)      
			for (int j = 0; j<secondM2_y; j++) secondM3[i][j]=(*pSecondM2)[i][j];
	}
	else
	{

		for (int i=0, mod=0, check=1; i<secondM2_x; i++)	{
			for (int k=1; k<thirdM_x; k++)	{
				if (i==0) break;
				if ( (*pSecondM2)[i][0] == (*pThirdM)[k][0])	{
					mod++;
					check=0;
					break;
				}
				else
				{
					check=1;
				}
			}
			if (check)	{
				for (int j=0; j<secondM2_y; j++)	{
					secondM3[i-mod][j]=(*pSecondM2)[i][j];
				}		
			}
		}

	}
	cout << "\n";
	cout << "Метод конструкшен";
	cout << "\n";
	cout << "SecondM3";
	cout << "\n";
	secondM3.show();

	secondM3.deleteNullCols();
	secondM3_y=secondM3.getY();

	secondM3.show();

	// ИНИЦИАЛИЗАЦИЯ БУДУЩИХ ГРАФОВ

	//Массив хранения графов

    vRes = new QVector<Matrix *>;

	//ИНИЦИАЛИЗАЦИЯ

	for (int j=1; j < secondM3_y; j++ )
	{
		int new_x=1, new_y=1;

		for (int l=0; l<setsM_y; l++)	{
			if ( (*pSetsM) [ secondM3[0][j] ] [l] )
				new_y++;
		}

		for (int i=1; i<secondM3_x; i++)
			if (secondM3[i][j])
				new_x++;


        (*vRes).append( new Matrix(new_x, new_y) );
        (*vRes)[vRes->size()-1] -> fillWithNumber(1);

	}


	//ЗАПОЛНЕНИЕ ГРАФОВЫХ ТАБЛИЦ ИЗ СЕКОНД М 3

	for (int j=1; j < secondM3_y; j++ )	{

       ( *(*vRes)[j-1])[0][0]=0;

		for (int b=0, gmod=1; b<setsM_y; b++)	{
			if ( (*pSetsM)  [ secondM3[0][j] ]  [ b ] )	{
                ( *(*vRes)[j-1])[0][gmod]=(*pMainM) [ headerM[b]+1 ]  [0]; //+1 Т.к индексные поля
				gmod++;
			}
		}

		for (int i=1, mod=1; i<secondM3_x; i++)	{
			if (secondM3[i][j])	{
                ( *(*vRes)[j-1])[mod][0]=secondM3[i][0];
				mod++;
			}
		}
	}



	//Вывод на экран графов
	cout << "/////////////////////////////////////////////////" << "\n";
	cout << "//////////////РEЗУЛЬТИРУЮЩИЕ ГРАФЫ  RES//////////" << "\n";
	cout << "/////////////////////////////////////////////////" << "\n";
	cout << "\n";
    for (int i=0; i < vRes->size(); i++)
        (*vRes)[i] -> show();
	cout << "/////////////////////////////////////////////////" << "\n";
}
