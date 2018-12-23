#include <stdio.h>
#include <time.h>
#include <algorithm>
#include <string>
#include <string.h>
#include "SEV.c"
#include "SEN.c"
using namespace std;

#define TRAINING_TRIALS 100
#define EVALUATION_TRIALS 100

void activationProcess(networkprogram &np)
{
	for(int i = 0; i<np.n.neurons.size(); ++i)
	{
		if(RANDOM_ACTIVATION)
		{
			if(rand() % DEFAULT_THRESHOLD < *np.n.neurons[i]->sum)
			{
				*np.n.neurons[i]->activated = 1;
				*np.n.neurons[i]->sum = 0;
			}
		}else{
			if(DEFAULT_THRESHOLD < *np.n.neurons[i]->sum)
			{
				*np.n.neurons[i]->activated = 1;
				*np.n.neurons[i]->sum = 0;
			}
		}
		
	}
}

void summationProcess(networkprogram &np)
{
	for(int i = 0; i<np.n.neurons.size(); ++i)
	{
		if(*np.n.neurons[i]->activated)
		{
			for(int x = 0; x<np.n.neurons[i]->outboundConnections.size(); ++x)
			{
				*np.n.neurons[np.n.neurons[i]->outboundConnections[x]->receiverNeuronID]->sum += *(np.n.neurons[i]->outboundConnections[x]->strength);
			}
			*np.n.neurons[i]->activated = 0;
		}
	}
}

void process(networkprogram &np)
{
//	summationProcess(np);
//	activationProcess(np);
//printf("START P %d %d\n",np.p.regs.size(),np.p.regs[0]);
	RunProgram(np.p);
//	printf("END P\n");
}

bool hasOutput(const network &n)
{
	for(int i = 0; i<n.outputNeurons.size(); ++i)
		if(*n.outputNeurons[i]->activated)
		return true;
	
	return false;
}


activationScheme listaS[NUM_ACTIVATION_SCHEMES];
void initSchemes()
{
	static activationScheme a1,a2,a3,a4,a5,a6,a7,a8;
	a1.input = {0,1};
	a1.output = {0,1};
	
	a2.input = {1,0};
	a2.output = {1,0};
	
	a3.input = {0,1};
	a3.output = {1,0};
	
	a4.input = {1,0};
	a4.output = {0,0};
	
	listaS[0] = a1;
	listaS[1] = a2;
	listaS[2] = a3;
	listaS[3] = a4;
}


vector<int> oraclate(networkprogram &np, const vector<int> input)
{
	if(input.size() != np.n.inputNeurons.size())
	return input;
	
	
	for(int i = 0; i<input.size(); ++i)
	{
		*np.n.inputNeurons[i]->activated = input[i];
	}
	
	
	
	
	for(int i = 0; i<100; ++i)
	{
		process(np);
		if(hasOutput(np.n))
		break;
	}
	
	vector<int> output;
	
	for(int i = 0; i<np.n.outputNeurons.size(); ++i)
	{
		output.push_back(*np.n.outputNeurons[i]->activated);
	}
	return output;
		
}

vector<networkprogram> listOracles;

void meta_oraclate(networkprogram &n)
{
	vector<int> output;
	for(int i = 0; i<NUM_ACTIVATION_SCHEMES; ++i)
	{
		for(int x = 0; x<TRAINING_TRIALS; ++x)
		{
			clearnet(n.n);
			output = oraclate(n,listaS[i].input);
		/*	printf("DESIRED OUTPUT:  ");
			for(int z = 0; z<listaS[i].output.size(); ++z)
			{
				printf("%d ",listaS[i].output[z]);
			}
			printf("\n");
			
			printf("RETURNED OUTPUT: ");
			for(int z = 0; z<listaS[i].output.size(); ++z)
			{
				printf("%d ",output[z]);
			}
			printf("\n\n");*/
			if(output == listaS[i].output)
			{
			//	printf("START EXE DP\n");
				networkprogramdp(n);
			//	printf("END EXE DP\n");
			}else{
			//	printf("START EXE ADP\n");
				networkprogramadp(n);
			//	printf("END EXE ADP\n");
			}	
		}
		
		
		for(int x = 0; x<EVALUATION_TRIALS; ++x)
		{
			clearnet(n.n);
			output = oraclate(n,listaS[i].input);
			
			if(output == listaS[i].output)
			{
				++n.scoretotal;
				++n.succs;
			}else{
				--n.scoretotal;
				++n.fails;
			}
		}
		
		resetnet(n.n);
	}
}



networkprogram prime;

program topp;
program topdp;
program topantidp;

void copyprograminstructions(const program &source, program &dest)
{
	dest.instructions.clear();
	for(int i = 0; i<source.instructions.size(); ++i)
	{
		dest.instructions.push_back(source.instructions[i]);
	}
}

#define POPULATION 1000

void meta_meta_oraclate()
{
	for(int ROTA = 0; ROTA < 1000; ++ROTA)
	{
		printf("ROTA %d\n",ROTA);
		for(int i = 0; i<listOracles.size(); ++i)
		{
			meta_oraclate(listOracles[i]);
		}
		
		sort(listOracles.begin(), listOracles.end());
		
		  copyprograminstructions(listOracles[0].p,topp);
		  copyprograminstructions(listOracles[0].dp,topdp);
		  copyprograminstructions(listOracles[0].antidp,topantidp);
		printf("TOP SCORE FOR GEN %d: %d(Succeeded %d times, Failed %d times)\n",ROTA,listOracles[0].scoretotal,listOracles[0].succs,listOracles[0].fails);
		string sfnP = "PROGRAMS\\TOP-P-" + to_string(ROTA) + ".txt";
		string sfnANTIDP = "PROGRAMS\\TOP-ANTIDP-" + to_string(ROTA) + ".txt";
		string sfnDP = "PROGRAMS\\TOP-DP-" + to_string(ROTA) + ".txt";
		
		saveProgram(topp,sfnP.c_str());
		saveProgram(topantidp,sfnANTIDP.c_str());
		saveProgram(topdp,sfnDP.c_str());
		
		for(int i = 0; i<listOracles.size(); ++i)
		{
			freenetworkprogram(listOracles[i]);
		}
		
		listOracles.clear();
		
		
		for(int i = 0; i<POPULATION; ++i)
		{
			networkprogram n;
			 copyprograminstructions(topdp,n.dp);
			 copyprograminstructions(topantidp,n.antidp);
			copyprograminstructions(topp,n.p);
			MUTATE_PROGRAM(n.dp);
			MUTATE_PROGRAM(n.antidp);
			MUTATE_PROGRAM(n.p);
			listOracles.push_back(n);
		/*	for(int z = 0; z<n.antidp.instructions.size(); ++z)
			{
				printf("%d\n",n.antidp.instructions[z]);
			}*/
			saveProgram(n.dp,"PROGRAMS\\MDP.txt");
			saveProgram(n.antidp,"PROGRAMS\\MANTIDP.txt");
			saveProgram(n.p,"PROGRAMS\\MP.txt");

		}
		
		
		networkprogram n;
		copyprograminstructions(topdp,n.dp);
		copyprograminstructions(topantidp,n.antidp);
		copyprograminstructions(topp,n.p);
		listOracles.push_back(n);
		
		
		
	}
	
	
}


int main()
{
	
	initSchemes();
	networkprogram n;
	
	/*for(int i = 0; i<n.n.neurons.size(); ++i)
	{
		for(int x = 0; x<n.n.neurons[i]->outboundConnections.size(); ++x)
		{
			for(int z = 0; z<n.regs.size(); ++z)
			{
				if(n.n.neurons[i]->outboundConnections[x]->strength == n.regs[z])
				printf("%d STRENGTH NEURON %d CONN %d\n",z,i,x);
			}
		}
		
		for(int z = 0; z<n.regs.size(); ++z)
		{
			if(n.n.neurons[i]->sum == n.regs[z])
			printf("%d SUM NEURON %d\n",z,i);
			if(n.n.neurons[i]->activated == n.regs[z])
			printf("%d ACTIVATED NEURON %d\n",z,i);
		}
		
	}
	*/
	
	char *primeantidp = (char*)malloc(30*sizeof(char));
	char *primedp = (char*)malloc(30*sizeof(char));
	char *primep = (char*)malloc(30*sizeof(char));
	int seed = 0;
	
	
	FILE *f = fopen("SETTINGS.txt","r");
	fscanf(f,"%d",&seed);
	fscanf(f,"%s",primep);
	fscanf(f,"%s",primedp);
	fscanf(f,"%s",primeantidp);
	fclose(f);
	
	if(seed == -1)
	{
		srand(time(0));
	}else{
		srand(seed);
	}
	
	
	loadProgram(primeantidp,n.antidp);
	loadProgram(primep,n.p);
	
	loadProgram(primedp,n.dp);
	
	free(primeantidp);
	free(primep);
	free(primedp);
		
	
	
	
	meta_oraclate(n);
	
	
	printf("SUCCS: %d FAILS: %d TOTALSCORE: %d\n",n.succs,n.fails,n.scoretotal);
	
	
	
}
