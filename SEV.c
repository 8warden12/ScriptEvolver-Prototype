#ifndef SEV_C
#define SEV_C
#include <vector>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <stdio.h>
using namespace std;


#define getVariableName(var) (#var)

#define IF 1000001
#define SQRT 1000002
#define ROOT 1000003
#define PWR 1000004
#define EXP 1000005
#define LOG 1000006
#define LN 1000007
#define EQ 1000008
#define GL 1000009
#define LL 1000010
#define GLE 1000011
#define LLE 1000012
#define EA 10000013
#define RT 10000014
#define INT 10000015
#define ADD 10000016
#define SUB 10000017
#define MUL 10000018
#define DIV 10000019
#define MOD 10000020
#define REG 10000021
#define SET 10000022
#define NEQ 10000023
#define RAND 10000024
#define EVPR 10000025
#define NVPR 10000026


#define DEFAULT_STRENGTH 100
#define DEFAULT_THRESHOLD 1000//change to 200
#define RANDOM_ACTIVATION 1

#define SYNTAXNUM 27

int SYNTAX[SYNTAXNUM] = {IF,SQRT,ROOT,PWR,EXP,LOG,LN,EQ,GL,LL,GLE,LLE,EA,RT,INT,ADD,SUB,MUL,DIV,MOD,REG,SET,NEQ,RAND,EVPR,NVPR};

typedef int INSTRUCTION;

#define MAXREG 150
#define MAXINT 1000000

typedef vector<int> INSTRUCTION_LIST;

struct program{
	vector<INSTRUCTION> instructions;
	vector<int*> regs;
	bool legal = false;
	int output = 0;
	
	program();
/*	{
		for(int i = 0; i<MAXREG; ++i)
		{
			regs.push_back(new int);
			*(regs[i]) = 0;
		}
	}*/
	
};

program::program()
{
	
}

struct connection{
	int *currentpassed = new int;
	int *strength = new int;
	int emitterNeuronID = 0;
	int receiverNeuronID = 0;
	int *SX0 = new int, *SX1 = new int, *SX2 = new int, *SX3 = new int, *SX4 = new int;
	
	int *SX[7] = {currentpassed,strength,SX0,SX1,SX2,SX3,SX4};
	
	connection();
};

struct neuron{
	int *activated = new int;
	int *sum = new int;
	vector<connection*> outboundConnections;
	vector<connection*> inboundConnections;
	int *NX0 = new int, *NX1 = new int, *NX2 = new int, *NX3 = new int;
	int *NX[6] = {NX0, NX1, NX2, NX3, activated,sum};
	
	neuron();
};

struct network{
	vector<neuron*> inputNeurons;
	vector<neuron*> outputNeurons;
	vector<neuron*> neurons;
	
};

struct networkprogram{
	
	network n;
	program p;
	program dp;
	program antidp;
	
	
//	vector<program> programs = {dp,antidp};	
	
	vector<int*> regs;
	
	int score1 = 0;
	int score2 = 0;
	int score3 = 0;
	int score4 = 0;
	int score5 = 0;
	int score6 = 0;
	int score7 = 0;
	int score8 = 0;
	
	int scoretotal = 0;
	int fails = 0;
	int succs = 0;
	inline bool operator<(const networkprogram &n1)
	{
		return n1.scoretotal < this->scoretotal ? 1 : 0; 
	}
	
	networkprogram();
};

void clearnet(network &n)
{
	for(int i = 0; i<n.neurons.size(); ++i)
	{
		*n.neurons[i]->activated = 0;
		*n.neurons[i]->sum = 0;
	}
}

void resetnet(network &n)
{
	for(int i = 0; i<n.neurons.size(); ++i)
	{
		for(int x = 0; x<n.neurons[i]->outboundConnections.size(); ++x)
		{
			for(int z = 0; z<7; ++z)
			{
				*(n.neurons[i]->outboundConnections[x]->SX[z]) = 0;
			}
			*(n.neurons[i]->outboundConnections[x]->strength) = DEFAULT_STRENGTH;
		}
		for(int x = 0; x<6; ++x)
		{
			*(n.neurons[i]->NX[x]) = 0;
		}
		
		*n.neurons[i]->activated = 0;
		*n.neurons[i]->sum = 0;
	}
}

vector<networkprogram> population;


void loadProgram(const char* filename, program &p)
{
	
	string s = "";
	char c;
	
	FILE *f = fopen(filename,"r");
	bool bn = false;
	while(c = getc(f))
	{
	//	printf("c == '%c'\n",c);
		if(isalpha(c) || isdigit(c))
		{
			
			s += c;
		}else if(c == EOF)
		{
		//	printf("s == %s\n",s.c_str());
			if(isdigit(s[0]))
			{
				p.instructions.push_back(stoi(s));
			}else if(s == "IF")
			{
				p.instructions.push_back(IF);
			}else if(s == "SQRT")
			{
				p.instructions.push_back(SQRT);
			}else if(s == "ROOT")
			{
				p.instructions.push_back(ROOT);
			}else if(s == "PWR")
			{
				p.instructions.push_back(PWR);
			}else if(s == "EXP")
			{
				p.instructions.push_back(EXP);
			}else if(s == "LOG")
			{
				p.instructions.push_back(LOG);
			}else if(s == "LN")
			{
				p.instructions.push_back(LN);
			}else if(s == "EQ")
			{
				p.instructions.push_back(EQ);
			}else if(s == "GL")
			{
				p.instructions.push_back(GL);
			}else if(s == "LL")
			{
				p.instructions.push_back(LL);	
			}else if(s == "GLE")
			{
				p.instructions.push_back(GLE);
			}else if(s == "LLE")
			{
				p.instructions.push_back(LLE);
			}else if(s == "EA")
			{
				p.instructions.push_back(EA);
			}else if(s == "RT")
			{
				p.instructions.push_back(RT);
			}else if(s == "INT")
			{
				p.instructions.push_back(INT);
			}else if(s == "ADD")
			{
				p.instructions.push_back(ADD);
			}else if(s == "SUB")
			{
				p.instructions.push_back(SUB);
			}else if(s == "MUL")
			{
				p.instructions.push_back(MUL);
			}else if(s == "DIV")
			{
				p.instructions.push_back(DIV);
			}else if(s == "MOD")
			{
				p.instructions.push_back(MOD);
			}else if(s == "REG")
			{
				p.instructions.push_back(REG);
			}else if(s == "SET")
			{
				p.instructions.push_back(SET);
			}else if(s == "NEQ")
			{
				p.instructions.push_back(NEQ);
			}else if(s == "RAND")
			{
				p.instructions.push_back(RAND);
			}else if(s == "EVPR")
			{
				p.instructions.push_back(EVPR);
			}else if(s == "NVPR")
			{
				p.instructions.push_back(NVPR);
			}
			break;
		}else{
		//	printf("s == %s\n",s.c_str());
			if(isdigit(s[0]))
			{
				p.instructions.push_back(stoi(s));
			}else if(s == "IF")
			{
				p.instructions.push_back(IF);
			}else if(s == "SQRT")
			{
				p.instructions.push_back(SQRT);
			}else if(s == "ROOT")
			{
				p.instructions.push_back(ROOT);
			}else if(s == "PWR")
			{
				p.instructions.push_back(PWR);
			}else if(s == "EXP")
			{
				p.instructions.push_back(EXP);
			}else if(s == "LOG")
			{
				p.instructions.push_back(LOG);
			}else if(s == "LN")
			{
				p.instructions.push_back(LN);
			}else if(s == "EQ")
			{
				p.instructions.push_back(EQ);
			}else if(s == "GL")
			{
				p.instructions.push_back(GL);
			}else if(s == "LL")
			{
				p.instructions.push_back(LL);	
			}else if(s == "GLE")
			{
				p.instructions.push_back(GLE);
			}else if(s == "LLE")
			{
				p.instructions.push_back(LLE);
			}else if(s == "EA")
			{
				p.instructions.push_back(EA);
			}else if(s == "RT")
			{
				p.instructions.push_back(RT);
			}else if(s == "INT")
			{
				p.instructions.push_back(INT);
			}else if(s == "ADD")
			{
				p.instructions.push_back(ADD);
			}else if(s == "SUB")
			{
				p.instructions.push_back(SUB);
			}else if(s == "MUL")
			{
				p.instructions.push_back(MUL);
			}else if(s == "DIV")
			{
				p.instructions.push_back(DIV);
			}else if(s == "MOD")
			{
				p.instructions.push_back(MOD);
			}else if(s == "REG")
			{
				p.instructions.push_back(REG);
			}else if(s == "SET")
			{
				p.instructions.push_back(SET);
			}else if(s == "NEQ")
			{
				p.instructions.push_back(NEQ);
			}else if(s == "RAND")
			{
				p.instructions.push_back(RAND);
			}else if(s == "EVPR")
			{
				p.instructions.push_back(EVPR);
			}else if(s == "NVPR")
			{
				p.instructions.push_back(NVPR);
			}
			
			s = "";
			
		}
		
/*
		if(c == EOF)
		{
			bn = true;
			c = ' ';
		}
		
		if(((c == ' ' || c == '\n' || c == '\t') && s != "" ) && s != " ")
		{
			if(isdigit(s[0]))
			{
				p.instructions.push_back(stoi(s));
			}
			
			printf("s == %s\n",s.c_str());
			
			if(s == "IF")
			{
				p.instructions.push_back(IF);
			}else if(s == "SQRT")
			{
				p.instructions.push_back(SQRT);
			}else if(s == "ROOT")
			{
				p.instructions.push_back(ROOT);
			}else if(s == "PWR")
			{
				p.instructions.push_back(PWR);
			}else if(s == "EXP")
			{
				p.instructions.push_back(EXP);
			}else if(s == "LOG")
			{
				p.instructions.push_back(LOG);
			}else if(s == "LN")
			{
				p.instructions.push_back(LN);
			}else if(s == "EQ")
			{
				p.instructions.push_back(EQ);
			}else if(s == "GL")
			{
				p.instructions.push_back(GL);
			}else if(s == "LL")
			{
				p.instructions.push_back(LL);	
			}else if(s == "GLE")
			{
				p.instructions.push_back(GLE);
			}else if(s == "LLE")
			{
				p.instructions.push_back(LLE);
			}else if(s == "EA")
			{
				p.instructions.push_back(EA);
			}else if(s == "RT")
			{
				p.instructions.push_back(RT);
			}else if(s == "INT")
			{
				p.instructions.push_back(INT);
			}else if(s == "ADD")
			{
				p.instructions.push_back(ADD);
			}else if(s == "SUB")
			{
				p.instructions.push_back(SUB);
			}else if(s == "MUL")
			{
				p.instructions.push_back(MUL);
			}else if(s == "DIV")
			{
				p.instructions.push_back(DIV);
			}else if(s == "MOD")
			{
				p.instructions.push_back(MOD);
			}else if(s == "REG")
			{
				p.instructions.push_back(REG);
			}else if(s == "SET")
			{
				p.instructions.push_back(SET);
			}else if(s == "NEQ")
			{
				p.instructions.push_back(NEQ);
			}else if(s == "RAND")
			{
				p.instructions.push_back(RAND);
			}else if(s == "EVPR")
			{
				p.instructions.push_back(EVPR);
			}else if(s == "NVPR")
			{
				p.instructions.push_back(NVPR);
			}
			
			s = "";
		}else if(c != ' ' && c != '\n' && c != '\t'){
			s += c;
		}
		
		if(bn)break;
		*/
	}
	
	
	fclose(f);
	
/*	for(int i = 0; i<p.instructions.size(); ++i)
	{
		printf("%d ",p.instructions[i]);
	}
*/	
	//p.instructions.pop_back();
//	printf("\n\n");
	
	/*for(int i = 0; i<p.instructions.size(); ++i)
	{
		printf("%d ",p.instructions[i]);
	}
*/	
	
}

void saveProgram(const program &p, const char* filename)
{
	FILE *f = fopen(filename,"w");
	
	for(int i = 0; i<p.instructions.size(); ++i)
	{
		switch(p.instructions[i])
		{
			case IF:
		//		printf("%d == %d IF\n",p.instructions[i],IF);
				fprintf(f,"\nIF ");
			break;
			
			case SQRT:
		//		printf("%d == %d SQRT\n",p.instructions[i],SQRT);
				fprintf(f,"SQRT ");
			break;
			
			case ROOT:
		//		printf("%d == %d ROOT\n",p.instructions[i],ROOT);
				fprintf(f,"ROOT ");
			break;
			
			case PWR:
		//		printf("%d == %d PWR\n",p.instructions[i],PWR);
				fprintf(f,"PWR ");
			break;
			
			case EXP:
	//			printf("%d == %d EXP\n",p.instructions[i],EXP);
				fprintf(f,"EXP ");
			break;
			
			case LOG:
	//			printf("%d == %d LOG\n",p.instructions[i],LOG);
				fprintf(f,"LOG ");
			break;
			
			case LN:
			//	printf("%d == %d LN\n",p.instructions[i],LN);
				fprintf(f,"LN ");
			break;
			
			case EQ:
			//	printf("%d == %d EQ\n",p.instructions[i],EQ);
				fprintf(f,"EQ ");
			break;
			
			case GL:
			//	printf("%d == %d GL\n",p.instructions[i],GL);
				fprintf(f,"GL ");
			break;
		
			case LL:
			//	printf("%d == %d LL\n",p.instructions[i],LL);
				fprintf(f,"LL ");
			break;
			
			case GLE:
			//	printf("%d == %d GLE\n",p.instructions[i],GLE);
				fprintf(f,"GLE ");
			break;
			
			case LLE:
			//	printf("%d == %d LLE\n",p.instructions[i],LLE);
				fprintf(f,"LLE ");
			break;
			
			case EA:
			//	printf("%d == %d EA\n",p.instructions[i],EA);
				fprintf(f,"\nEA\n");
			break;
			
			case RT:
			//	printf("%d == %d RT\n",p.instructions[i],RT);
				fprintf(f,"\nRT\n");
			break;
			
			case INT:
			//	printf("%d == %d INT\n",p.instructions[i],INT);
				fprintf(f,"INT ");
			break;
			
			case ADD:
			//	printf("%d == %d ADD\n",p.instructions[i],ADD);
				fprintf(f,"ADD ");
			break;
			
			case SUB:
			//	printf("%d == %d SUB\n",p.instructions[i],SUB);
				fprintf(f,"SUB ");
			break;
			
			case MUL:
			//	printf("%d == %d MUL\n",p.instructions[i],MUL);
				fprintf(f,"MUL ");
			break;
			
			case DIV:
			//	printf("%d == %d DIV\n",p.instructions[i],DIV);
				fprintf(f,"DIV ");
			break;
			
			case MOD:
			//	printf("%d == %d MOD\n",p.instructions[i],MOD);
				fprintf(f,"MOD ");
			break;
			
			case REG:
				if((p.instructions[i + 2] == SQRT || p.instructions[i + 2] == LOG) || (p.instructions[i + 2] == EXP || p.instructions[i + 2] == LN))
				{
			//		printf("%d == %d REG\n",p.instructions[i],REG);
					fprintf(f,"\nREG ");
				}else if(((p.instructions[i + 4] == ADD || p.instructions[i + 4] == SUB) || (p.instructions[i + 4] == MUL || p.instructions[i + 4] == DIV)) || (p.instructions[i + 4] == MOD || p.instructions[i + 4] == ROOT))
				{
			//		printf("%d == %d REG\n",p.instructions[i],REG);
					fprintf(f,"\nREG ");
				}else{
			//		printf("%d == %d REG\n",p.instructions[i],REG);
					fprintf(f,"REG ");
				}
				
				//	printf("%d == %d REG\n",p.instructions[i],REG);
				//	fprintf(f,"REG ");
				
			break;
			
			case SET:
		//		printf("%d == %d SET\n",p.instructions[i],SET);
				fprintf(f,"\nSET ");
			break;
			
			case NEQ:
		//		printf("%d == %d NEQ\n",p.instructions[i],NEQ);
				fprintf(f,"NEQ ");
			break;
			
			case RAND:
		//		printf("%d == %d RAND\n",p.instructions[i],RAND);
				fprintf(f,"RAND ");
			break;
			
			case EVPR:
		//		printf("%d == %d EVPR\n",p.instructions[i],EVPR);
				fprintf(f,"\nEVPR\n");
			break;
			
			case NVPR:
		//		printf("%d == %d NVPR\n",p.instructions[i],NVPR);
				fprintf(f,"\nNVPR\n");
			break;
			
			default:
				fprintf(f,"%d ",p.instructions[i]);
			break;
			
		}
	}
	
	
	fclose(f);
}

void saveProgramUNR(const program &p, const char* filename)
{
	for(int i = 0; i<p.instructions.size(); ++i)
	{
	}	
	
}


struct compmessage{
	int line = -1;
	int code = 0;
	string message = "This should not be seen.\n";
};

//0: successful compilation
//-1: no RETURN at end of program
//-2: different number of IFs and EAs
//-3: General syntax error
//-4: not enough symbols
compmessage CompileProgram(program &p)
{
	
	compmessage rm;
	
	/*if(p.instructions[p.instructions.size()-3] != RT)
	{
		rm.code = -1;
		rm.line = p.instructions.size()-1;
		rm.message = "return (RT) statement not found at end of program.\n";
		return rm;
	}*/
	
	int ifs = 0;
	
	for(int i = 0; i<p.instructions.size(); ++i)
	switch(p.instructions[i])
	{
		case IF:
			++ifs;
		break;
		
		case EA:
			--ifs;
		break;
		
		default:
		break;
	}
	
	if(ifs)
	{
		rm.code = -2;
		return rm;
	}
	
	
	for(int i = 0; i<p.instructions.size(); ++i)
	{
		switch(p.instructions[i])
		{
			case IF:
				if(i+5 >= p.instructions.size())
				{
					rm.line = i+5;
					rm.code = -4;
					return rm;
				}
				
				if(!((p.instructions[i+1] == INT || p.instructions[i+1] == RAND) || (p.instructions[i+1] == REG && p.instructions[i+2] <= MAXREG && p.instructions[i+2] >= 0)))
				{printf("errorhere");
					rm.code = -3;
					rm.line = i+1;
					return rm;
				}
				
				if(!((p.instructions[i+3] == INT || p.instructions[i+3] == RAND) || (p.instructions[i+3] == REG && p.instructions[i+4] <= MAXREG && p.instructions[i+4] >= 0)))
				{printf("errorhere");
					rm.code = -3;
					rm.line = i+3;
					return rm;	
				}
				
				if(!(p.instructions[i+5] == NEQ || p.instructions[i+5] == EQ || p.instructions[i+5] == GL || p.instructions[i+5] == LL || p.instructions[i+5] == GLE || p.instructions[i+5] == LLE))
				{printf("errorhere");
					rm.code = -3;
					rm.line = i+5;
					return rm;
				}
				
				i += 5;
			break;
			
			case REG:
				
				if(i+2 >= p.instructions.size())
				{
					rm.code = -4;
					rm.line = i+2;
					return rm;
				}
				
				if(!(p.instructions[i+1] >= 0 && p.instructions[i+1] <= MAXREG))
				{
					rm.code = -3;
					rm.line = i+1;
					return rm;
				}
				
				if(p.instructions[i+2] == REG)
				{
					if(i+4 >= p.instructions.size())
					{
						rm.code = -4;
						rm.line = i+4;
						return rm;
					}
					
					if(!(p.instructions[i+3] >= 0 && p.instructions[i+3] <= MAXREG))
					{
						rm.code = -3;
						rm.line = i+3;
						return rm;
					}
					
					switch(p.instructions[i+4])
					{
						case ADD:
							
						break;
						
						case SUB:
							
						break;
						
						case MUL:
							
						break;
						
						case DIV:
							
						break;
						
						case MOD:
							
						break;
						
						case ROOT:
							
						break;
						
						default:
							rm.code = -3;
							rm.line = i+4;
							return rm;
						break;	
						
					}
					i += 3;
				}else if(p.instructions[i+2] == INT)
				{
					if(i+4 >= p.instructions.size())
					{
						rm.code = -4;
						rm.line = i+4;
						return rm;
					}
					
					switch(p.instructions[i+4])
					{
						case ADD:
							
						break;
						
						case SUB:
							
						break;
						
						case MUL:
							
						break;
						
						case DIV:
							
						break;
						
						case MOD:
							
						break;
						
						case ROOT:
							
						break;
						
						default:
							rm.code = -3;
							rm.line = i+4;
							return rm;
						break;	
						
					}
					i += 3;
					
				}else if(p.instructions[i+2] == SQRT)
				{
					
				}else if(p.instructions[i+2] == RT)
				{
					
				}else if(p.instructions[i+2] == EXP)
				{
					
				}else if(p.instructions[i+2] == LN)
				{
					
				}else{
					rm.code = -3;
					rm.line = i+2;
					return rm;
				}
				
				i += 1;
			break;
			
			case SET:
				
				if(i+4 >= p.instructions.size())
				{
					rm.code = -4;
					rm.line = i+4;
					return rm;
				}
				
				if(p.instructions[i+1] != REG)
				{
					rm.code = -3;
					rm.line = i+1;
					return rm;
				}
				
				if(!(p.instructions[i+2] >= 0 && p.instructions[i+2] <= MAXREG))
				{
					rm.code = -3;
					rm.line = i+2;
					return rm;
				}
				
				if(!(p.instructions[i+3] == REG || p.instructions[i+3] == INT))
				{
					rm.code = -3;
					rm.line = i+3;
					return rm;
				}
				
				if(p.instructions[i+3] == REG)
				{
					if(!(p.instructions[i+4] >= 0 && p.instructions[i+4] <= MAXREG))
					{
						rm.code = -3;
						rm.line = i+4;
						return rm;
					}
				}
				
				i += 4;
				
			break;
			
			case RT:
				if(i+2 >= p.instructions.size())
				{
					rm.code = -4;
					rm.line = i+2;
					return rm;
				}
				
				if(p.instructions[i+1] == INT)
				{
					
				}else if(p.instructions[i+1] == REG)
				{
					if(!(p.instructions[i+2] >= 0 && p.instructions[i+2] <= MAXREG))
					{
						rm.code = -3;
						rm.line = i+2;
						return rm;
					}
				}else{
					rm.code = -3;
					rm.line = i+1;
					return rm;
				}
				
				i += 1;
				
				
			break;
			
			case EA:
				
			break;
		}
	}
	
	rm.code = 0;
	rm.line = -1;
	rm.message = "Compilation Successful";
	p.legal = true;
	return rm;
	
}

int RunProgram(program &p)
{
	int fci = 0;
	int sci = 0;
	int regint = 0;
	int regint2 = 0;
	int ifp = 0;
	bool proceed = false;
	for(int i = 0; i<p.instructions.size(); ++i)
	{
		switch(p.instructions[i])
		{
			case IF:
				fci = 0;
				sci = 0;
				proceed = false;
				if(p.instructions[i+1] == INT)
				{
					fci = p.instructions[i+2];
				}else if(p.instructions[i+1] == REG)
				{
					fci = *p.regs[p.instructions[i+2]];
				}else if(p.instructions[i+1] == RAND)
				{
					if(p.instructions[i+2] < 0)
					{
						fci = rand() % (p.instructions[i+2]*-1);
						fci *= -1;  
					}else if(p.instructions[i+2] == 0)
					{
						fci = 0;
					}else{
						fci = rand() % p.instructions[i+2];
					}
				}
				
				if(p.instructions[i+3] == INT)
				{
					sci = p.instructions[i+4];
				}else if(p.instructions[i+3] == REG)
				{
					sci = *p.regs[p.instructions[i+4]];
				}else if(p.instructions[i+3] == RAND)
				{
					if(p.instructions[i+4] < 0)
					{
						sci = rand() % (p.instructions[i+4]*-1);
						sci *= -1;  
					}else if(p.instructions[i+4] == 0)
					{
						sci = 0;
					}else{
						sci = rand() % p.instructions[i+4];
					}
				}
				
				switch(p.instructions[i+5])
				{
					case EQ:
						if(fci == sci)
							proceed = true;
					break;
					
					case NEQ:
						if(fci != sci)
							proceed = true;
					break;
					
					case GL:
						if(fci > sci)
							proceed = true;
					break;
					
					case GLE:
						if(fci >= sci)
							proceed = true;
					break;
					
					case LL:
						if(fci < sci)
							proceed = true;
					break;
					
					case LLE:
						if(fci <= sci)
							proceed = true;
					break;
				}
				
				ifp = 0;
				
				if(!proceed)
				{
					for(int x = i; x<p.instructions.size(); ++x)
					{
						if(p.instructions[x] == IF)
						{
							++ifp;
						}else if(p.instructions[x] == EA)
						{
							--ifp;
						}
						if(ifp == 0)
						{
							i = x;
							break;
						}
					}
				}else{
					i += 4;
				}
				
				
				
			break;
			
			case REG:
				
				regint = *p.regs[p.instructions[i+1]];

				if(p.instructions[i+2] == REG)
				{
					regint2 = *p.regs[p.instructions[i+3]];
					
					switch(p.instructions[i+4])
					{
						case ADD:
							regint += regint2;
						break;
						
						case SUB:
							regint -= regint2;
						break;
						
						case MUL:
							regint *= regint2;
						break;
						
						case DIV:
							if(regint2 == 0)
							{
								regint = 0;
							}else{
								regint /= regint2;
							}
							
						break;
						
						case MOD:
							if(regint2 == 0)
							{
								regint = 0;
							}else{
								regint %= regint2;
							}
						break;
						
						case ROOT:
							if(regint2 == 0)
							{
								regint = 0;
							}else{
								regint = pow((double)regint, 1.0/(double)regint2);
							}
						break;
					}
					
					*p.regs[p.instructions[i+1]] = regint;
					
					
				}else if(p.instructions[i+2] == INT)
				{
					regint2 = p.instructions[i+3];
					
					switch(p.instructions[i+4])
					{
						case ADD:
							regint += regint2;
						break;
						
						case SUB:
							regint -= regint2;
						break;
						
						case MUL:
							regint *= regint2;
						break;
						
						case DIV:
							if(regint2 == 0)
							{
								regint = 0;
							}else{
								regint /= regint2;
							}
						break;
						
						case MOD:
							if(regint2 == 0)
							{
								regint = 0;
							}else{
								regint %= regint2;
							}
						break;
						
						case ROOT:
							if(regint2 == 0)
							{
								regint = 0;
							}else{
								regint = pow((double)regint, 1.0/(double)regint2);
							}
						break;
						
					}
					
					*p.regs[p.instructions[i+1]] = regint;
					
				}else if(p.instructions[i+2] == SQRT)
				{
					*p.regs[p.instructions[i+1]] = (int)sqrt((double)*p.regs[p.instructions[i+1]]);
				}else if(p.instructions[i+2] == RT)
				{
					return *p.regs[p.instructions[i+1]];
				}else if(p.instructions[i+2] == EXP)
				{
					*p.regs[p.instructions[i+1]] = (int)exp((double)*p.regs[p.instructions[i+1]]);
				}else if(p.instructions[i+2] == LN)
				{
					*p.regs[p.instructions[i+1]] = (int)log((double)*p.regs[p.instructions[i+1]]);
				}else if(p.instructions[i+2] == LOG)
				{
					*p.regs[p.instructions[i+1]] = (int)log10((double)*p.regs[p.instructions[i+1]]);
				}
				
				i += 1;
				
			break;
			
			case SET:
				if(p.instructions[i+3] == INT)
				{
					*p.regs[p.instructions[i+2]] = p.instructions[i+4];
				}else if(p.instructions[i+3] == REG){
					*p.regs[p.instructions[i+2]] = *p.regs[p.instructions[i+4]];
				}
				i += 4;
				
			break;
			
			case RT:
			
				
				if(p.instructions[i+1] == INT)
				{
					return p.instructions[i+2];
				}else if(p.instructions[i+1] == REG)
				{
					return *p.regs[p.instructions[i+2]];
				}
				
				i += 1;
				
			break;
			
			case EA:
				
			break;
		}
	}
	
	
	
}

void MUT_REG(program &p, int index)
{
	
	int prev = p.instructions[index];
	
	switch(rand()%10)
	{
		case 0:
			p.instructions[index] += rand()%5 - 2;
		break;
		
		case 1:
			p.instructions[index] += rand()%11 - 5;
		break;
		
		case 2:
			p.instructions[index] += rand()%21 - 10;
		break;
		
		case 3:
			p.instructions[index] += rand()%51 - 25;
		break;
	}

	if(p.instructions[index] < 0 || p.instructions[index] >= MAXREG)
	p.instructions[index] = prev;
	
	if(p.instructions[index] < 0 || p.instructions[index] >= MAXREG)
	p.instructions[index] = rand()%150;
	
//	printf("PIN - %d - PIN\n",p.instructions[index]);
	
}

void MUT_INT(program &p, int index)
{
	
	switch(rand()%10)
	{
		case 0:
			p.instructions[index] += rand()%11 - 5;
		break;
		
		case 1:
			p.instructions[index] += rand()%101 - 50;
		break;
		
		case 2:
			p.instructions[index] += rand()%501 - 250;
		break;
		
		case 3:
			p.instructions[index] += rand()%1001 - 500;
		break;
	}
}



void verifix(program &p)
{
	
}

void networkprogramdp(networkprogram &n)
{
	RunProgram(n.dp);
}

void networkprogramadp(networkprogram &n)
{
	RunProgram(n.antidp);
}

int COMPINS[6] = {EQ,NEQ,GL,LL,GLE,LLE};
int VHINS[3] = {REG,INT,RAND};
int SINOPINS[4] = {SQRT,LOG,LN,EXP};
int DOUBOPINS[6] = {ROOT,ADD,SUB,MUL,DIV,MOD};
int IMPINS[3] = {REG,SET,IF};

bool isProtected(int index, const program& p)
{
	for(index; index<p.instructions.size(); ++index)
	{
		if(p.instructions[index] == NVPR)
		{
			return true;
		}else if(p.instructions[index] == EVPR)
		{
			return false;
		}
	}
	return false;	
}

bool isImpReg(int index, const program& p)
{
	if(index + 4 >= p.instructions.size())
	{
		return false;
	}
	
	if((p.instructions[index + 2] == SQRT || p.instructions[index + 2] == LOG) || (p.instructions[index + 2] == EXP || p.instructions[index + 2] == LN))
	{
		return true;
	}else if(((p.instructions[index + 4] == ADD || p.instructions[index + 4] == SUB) || (p.instructions[index + 4] == MUL || p.instructions[index + 4] == DIV)) || (p.instructions[index + 4] == MOD || p.instructions[index + 4] == ROOT))
	{
		return true;
	}
	return false;
}

int closestbs(int index, const program& p)
{
	for(index; index<p.instructions.size(); ++index)
	{
		if(!isProtected(index,p))
		if(p.instructions[index] == IF)
		{
			return index;
		}else if(p.instructions[index] == REG && isImpReg(index,p))
		{
			return index;
		}else if(p.instructions[index] == EA)
		{
			return index;
		}else if(p.instructions[index] == SET)
		{
			return index;
		}else if(p.instructions[index] == EVPR)
		{
			return index;
		}
	}
	return -1;
}

int closestDELETEs(int index, const program& p)
{
	for(index; index<p.instructions.size(); ++index)
	{
		if(!isProtected(index,p))
		{
			if(p.instructions[index] == IF)
			{
				return index;
			}
			if(p.instructions[index] == REG && isImpReg(index,p))
			{
				return index;
			}
			if(p.instructions[index] == SET)
			{
				return index;
			}
			
		}
		
	}
	return -1;
}

void insertRandomStatement(int index, program &p)
{
	
	vector<int> imps;
	switch(rand()%2)//NO IF FOR NOW
	{
		case 0:
			imps.push_back(SET);
			imps.push_back(REG);
			imps.push_back(rand()%150);
			imps.push_back(rand()%2 == 0 ? REG : INT);
			imps.push_back(rand()%150);
		break;
		
		case 1:
			imps.push_back(REG);
			imps.push_back(rand()%150);
			if(rand()%2 == 0)
			{
				imps.push_back(SINOPINS[rand()%4]);
			}else{
				imps.push_back(rand()%2 == 0 ? REG : INT);
				imps.push_back(rand()%150);
				imps.push_back(DOUBOPINS[rand()%6]);
			}
		break;
		
		case 2:
			imps.push_back(IF);
			imps.push_back(VHINS[rand()%3]);
			imps.push_back(rand()%150);
			imps.push_back(rand()%2 == 0 ? REG : INT);
			imps.push_back(rand()%150);
			imps.push_back(COMPINS[rand()%6]);
			imps.push_back(EA);
		break;
	}
	
	
	for(int i = 0; i<imps.size(); ++i)
	{
		p.instructions.insert(p.instructions.begin()+index+i,imps[i]);
	}
}

void deleteStatement(int index, program &p)
{
	int o = 0;
	switch(p.instructions[index])
	{
		case IF:
			for(int i = index; i < p.instructions.size(); ++i)
			{
				if(p.instructions[i] == IF)
				++o;
				if(p.instructions[i] == EA)
				--o;
				
				if(!o)
				{
					p.instructions.erase(p.instructions.begin() + i);
					break;	
				}	
			}
			
			p.instructions.erase(p.instructions.begin() + index,p.instructions.begin() + index + 6);
			
		break;
		
		case SET:
			p.instructions.erase(p.instructions.begin() + index, p.instructions.begin() + index + 5);
		break;
		
		case REG:
			if(p.instructions[index + 2] == SQRT || p.instructions[index + 2] == EXP || p.instructions[index + 2] == LOG || p.instructions[index + 2] == LN)
			{
				p.instructions.erase(p.instructions.begin() + index, p.instructions.begin() + index + 3);
			}else{
				p.instructions.erase(p.instructions.begin() + index, p.instructions.begin() + index + 5);
			}
		break;
	}
}

void MUTATE_PROGRAM(program &p)
{
	int pr = 0;
	for(int i = 0; i<p.instructions.size(); ++i)
	{
		if(p.instructions[i] == EVPR)
		{
			++pr;
		}else if(p.instructions[i] == NVPR)
		{
			--pr;
		}
		
		if(pr == 0)
		if(p.instructions[i] == INT || (p.instructions[i] == REG && !isImpReg(i,p) && p.instructions[i-1] != SET))
		{
			switch(rand()%10)
			{
				case 0:
					p.instructions[i] = INT;
					MUT_INT(p,i+1);
				break;
				
				case 1:
					MUT_REG(p,i+1);
					p.instructions[i] = REG;
				break;
			}
		}else if(p.instructions[i] == IF)
		{
			switch(rand()%10)
			{
				case 0:
					p.instructions[i+1] = RAND;
					MUT_INT(p,i+2);
				break;
				
				case 1:
					p.instructions[i+1] = INT;
					MUT_INT(p,i+2);
				break;
				
				case 2:
					p.instructions[i+1] = REG;
					MUT_REG(p,i+2);
				break;
			}
		}else if(p.instructions[i] == EQ || p.instructions[i] == NEQ || p.instructions[i] == GL || p.instructions[i] == LL || p.instructions[i] == GLE || p.instructions[i] == LLE)
		{
			if(rand()%20 == 0)
				p.instructions[i] = COMPINS[rand()%6];
		}else if(p.instructions[i] == SQRT || p.instructions[i] == LOG || p.instructions[i] == LN || p.instructions[i] == EXP)
		{
			if(rand()%20 == 0)
				p.instructions[i] = SINOPINS[rand()%4];
		}else if(p.instructions[i] == ROOT || p.instructions[i] == ADD || p.instructions[i] == SUB || p.instructions[i] == MUL || p.instructions[i] == DIV || p.instructions[i] == MOD)
		{
			if(rand()%20 == 0)
				p.instructions[i] = DOUBOPINS[rand()%6];
		}
	}
	
	int statementsToDelete = rand()%10;
	int statementsToAdd = rand()%10;
	int randomIndex = 0;
	
	for(int i = 0; i<statementsToAdd; ++i)
	{
		randomIndex = closestbs(randomIndex = rand()%p.instructions.size(),p);
		if(randomIndex != -1)
		{
			insertRandomStatement(randomIndex,p);
		}
	}
	
	for(int i = 0; i<statementsToDelete; ++i)
	{
		randomIndex = closestDELETEs(randomIndex = rand()%p.instructions.size(), p);
		if(randomIndex != -1)
		deleteStatement(randomIndex,p);
	}
	
	
	
}


#endif
