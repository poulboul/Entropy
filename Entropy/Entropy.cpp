#include <string>
#include <iostream>
#include <fstream>
#include <bitset>
#include <cmath>

using namespace std;

string ReadFile(string line);
string Levinshtein(string line);
string AddingRemainder(string lastLine, int blockLength);
string* BlockGrabber(string lastLine, string* blocksArray, int blockLength, string block, int sizeBlocksArray);
int* FrequencyCounter(string* blocksArray, int sizeBlocksArray, string lastLine, int blockLength);
double* PropabilityArray(int* frequencys, int sizeBlocksArray);
double* Entropy(double* propabilitys, int sizeBlocksArray, int* frequencys);


int main()
{
	int blockLength;
	cout << "Enter block length " << endl;
	cin >> blockLength;
	string line; // строка для записи из файла
	string showLine; // оригинальная строка для демонстрации index -> SH
	string lastLine; // строка для алгоритма Левинштейна
	line = ReadFile(line);
	showLine = line;
	int SH_sizeBlocksArray = (showLine.size() / blockLength);

	// Демонстрация энтропии файла до кодирования
	cout << "Code before codder " << showLine << endl;
	showLine = AddingRemainder(showLine, blockLength);
	string* SH_blocksArray = new string[SH_sizeBlocksArray];
	string SH_block = "."; // строка для взятия необходимого количества битов 
	string* SH_blocks = BlockGrabber(showLine, SH_blocksArray, blockLength, SH_block, SH_sizeBlocksArray);
	int* SH_frequencys = FrequencyCounter(SH_blocksArray, SH_sizeBlocksArray, showLine, blockLength);
	double* SH_propabilitys = PropabilityArray(SH_frequencys, SH_sizeBlocksArray);
	Entropy(SH_propabilitys, SH_sizeBlocksArray, SH_frequencys);

	// Демонстрация энтропии файла после кодирования
	lastLine = Levinshtein(line);
	lastLine = AddingRemainder(lastLine, blockLength);
	int sizeBlocksArray = (lastLine.size() / blockLength);
	string* blocksArray = new string[sizeBlocksArray];
	string block = "."; // строка для взятия необходимого количества битов 
	string* blocks = BlockGrabber(lastLine, blocksArray, blockLength, block, sizeBlocksArray);
	int* frequencys = FrequencyCounter(blocksArray, sizeBlocksArray, lastLine, blockLength);
	double* propabilitys = PropabilityArray(frequencys, sizeBlocksArray);
	Entropy(propabilitys, sizeBlocksArray, frequencys);
	return 0;
}

string ReadFile(string line)
{
	ifstream out("./file.txt"); // окрываем файл для чтения
	if (out.is_open())
	{
		while (getline(out, line))
		{
			cout << line << endl;
		}
	}
	else
	{
		cout << "ERROR" << endl;
	}
	out.close(); // закрываем файл
	return (line);
};

string Levinshtein(string line)
{
	int sizeArray = line.size();
	string secondLine; // строка для обратной записи line
	string lastLine; // строка для алгоритма Левинштейна 
	for (int i = 0; i < sizeArray; i++)
	{
		secondLine[i] = line[sizeArray - i - 1];
		cout << secondLine[i];
	}
	cout << endl;
	for (int i = 0; i < (sizeArray); i++)
	{
		if (i < sizeArray - 1)
		{
			lastLine += '0';
			lastLine += secondLine[i];
		}
		else
		{
			lastLine += secondLine[i];
		}
	}
	cout << lastLine << endl;
	cout << endl;
	return (lastLine);
}

string* BlockGrabber(string lastLine, string* blocksArray, int blockLength, string block, int sizeBlocksArray)
{
	int indexHelp = 0;
	int indexArray = 0;
	int counter = 0;
	for (int i = 0; i < lastLine.size(); i += blockLength)
	{
		if (indexArray < lastLine.size())
		{
			for (int j = i; j < i + blockLength;)
			{
				if (indexHelp == 0)
				{
					block = lastLine[j];
				}
				else
				{
					block += lastLine[j];
				}
				j++;
				indexHelp++;
			};
			for (int j = 0; j < sizeBlocksArray; j++)
			{
				if (block != blocksArray[j])
				{
					counter++;
				}
				else
				{
					block = "";
					counter = 0;
				};
			}
			if (counter == sizeBlocksArray)
			{
				blocksArray[indexArray] = block;
				cout << blocksArray[indexArray] << endl;
				indexArray++;
				block = "";
				counter = 0;
			}
		}
	};
	return (blocksArray);
};

string AddingRemainder(string lastLine, int blockLength)
{
	int remains = lastLine.size() % blockLength;
	int supplement = 0; // Количество бит, которое необходимо добавить
	if (remains > 0)
	{
		int magnifer = lastLine.size();
		while (remains != 0)
		{
			magnifer++;
			remains = magnifer % blockLength;
			supplement++;
		}
		lastLine.insert(0, supplement, '0');
		return(lastLine);
	}
	else
	{
		return (lastLine);
	};
};

int* FrequencyCounter(string* blocksArray, int sizeBlocksArray, string lastLine, int blockLength)
{
	string block = "";
	int indexHelp = 0;
	int frequency = 0;
	int* frequencyArray = new int[sizeBlocksArray];
	for (int i = 0; i < sizeBlocksArray; i++)
	{
		block = blocksArray[i];
		for (int j = 0; j < lastLine.size();)
		{
			if ((indexHelp < blockLength) and (block[indexHelp] == lastLine[j]))
			{
				indexHelp++;
				j++;
			}
			else if (indexHelp == blockLength)
			{
				frequency++;
				indexHelp = 0;
			}
			else if ((indexHelp < blockLength) and (block[indexHelp] != lastLine[j]))
			{
				j += blockLength - indexHelp;
				indexHelp = 0;
			}
		};
		frequencyArray[i] = frequency;
		if (frequencyArray[i] == 0)
		{
			blocksArray[i] = "Empty block";
		}
		else
		{
			cout << "Block " << blocksArray[i] << " repeated " << frequencyArray[i] << " times;" << endl;
		};
		frequency = 0;
	};
	return(frequencyArray);
};

double* PropabilityArray(int* frequencys, int sizeBlocksArray)
{
	double* propabilityArray = new double[sizeBlocksArray];
	double size = sizeBlocksArray;
	for (int i = 0; i < sizeBlocksArray; i++)
	{
		if (frequencys[i] == 0)
		{

		}
		else
		{
			propabilityArray[i] = frequencys[i] / size;
			cout << "propability for " << i << "st block is " << propabilityArray[i] << endl;
		}
	};
	return (propabilityArray);
};

double* Entropy(double* propabilitys, int sizeBlocksArray, int* frequencys)
{
	double entropy = 0;
	double value;
	for (int i = 0; i < sizeBlocksArray; i++)
	{
		if (frequencys[i] == 0) {}
		else
		{
			//cout << "log2 = " << log2(propabilitys[i]) << endl;
			if (log2(propabilitys[i]) > 0)
			{
				value = round(log2(propabilitys[i]) * 100) / 100;
				entropy += propabilitys[i] * value;
			}
			else if (log2(propabilitys[i]) < 0)
			{
				value = round(-log2(propabilitys[i]) * 100) / 100;
				entropy += propabilitys[i] * value;
			}
		};
	}
	cout << "entropy = " << entropy << endl;
	return 0;
}

