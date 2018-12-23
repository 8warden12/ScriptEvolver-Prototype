#ifndef SEN_C
#define SEN_C

#include "SEV.c"
#include <vector>
using namespace std;

struct activationScheme{
	vector<int> input;
	vector<int> output;
};

#define NUM_ACTIVATION_SCHEMES 4


connection::connection()
{
	*(this->strength) = DEFAULT_STRENGTH;
}



neuron::neuron()
{
	for(int i = 0; i<6; ++i)
	{
		*(this->NX[i]) = 0;
	}
}



void connect(int emitterNeuronID, int receiverNeuronID, network &n)
{
	connection *c = new connection;
	c->emitterNeuronID = emitterNeuronID;
	c->receiverNeuronID = receiverNeuronID;
	n.neurons[emitterNeuronID]->outboundConnections.push_back(c);
	n.neurons[receiverNeuronID]->inboundConnections.push_back(c);

}




//use manual configuration for now
networkprogram::networkprogram()
{
	neuron *n1 = new neuron, *n2 = new neuron, *n3 = new neuron, *n4 = new neuron, *n5 = new neuron, *n6 = new neuron, *n7 = new neuron, *n8 = new neuron;
	this->n.neurons = {n1,n2,n3,n4,n5,n6,n7,n8};
	
	
	this->n.inputNeurons = {n1,n2};
	this->n.outputNeurons = {n7,n8};
	
	for(int i = 0; i<MAXREG; ++i)
	{
		this->regs.push_back(NULL);
		this->dp.regs.push_back(NULL);
		this->antidp.regs.push_back(NULL);
		this->p.regs.push_back(NULL);
		//*(this->regs[i]) = 0;
	}	
	
	

	
	
	connect(0,2,this->n);
	connect(0,3,this->n);
	connect(1,2,this->n);
	connect(1,3,this->n);
	connect(2,4,this->n);
	connect(2,5,this->n);
	connect(3,4,this->n);
	connect(3,5,this->n);
	connect(4,6,this->n);
	connect(4,7,this->n);
	connect(5,6,this->n);
	connect(5,7,this->n);
	
	int REGcounter = 0;
	
	for(int i = 0; i<this->n.neurons.size(); ++i)
	{
		for(int x = 0; x < 6; ++x)
		{
		//	if(this->n.neurons[i]->NX[x] == this->n.neurons[i]->activated)
		//	printf("n%d->activated REG: %d\n",i+1,REGcounter);
			
		//	if(this->n.neurons[i]->NX[x] == this->n.neurons[i]->sum)
		//	printf("n%d->sum REG: %d\n",i+1,REGcounter);
			
			this->regs[REGcounter++] = this->n.neurons[i]->NX[x];
		}
		
	//	printf("\n");	
		
		for(int x = 0; x<this->n.neurons[i]->outboundConnections.size(); ++x)
		{
			for(int z = 0; z<7; ++z)
			{
			//	if(this->n.neurons[i]->outboundConnections[x]->SX[z] == this->n.neurons[i]->outboundConnections[x]->strength)
			//	printf("n%d c%d->strength going to n%d REG: %d\n",i+1,x+1,this->n.neurons[i]->outboundConnections[x]->receiverNeuronID+1,REGcounter);
				
				this->regs[REGcounter++] = this->n.neurons[i]->outboundConnections[x]->SX[z];
				
			}
				
		}
		
	//	printf("\n\n");
			 
	}
	
	for(REGcounter; REGcounter < MAXREG; ++REGcounter)
	{
		this->regs[REGcounter] = new int;
	}
	
	
	for(int x = 0; x<this->regs.size(); ++x)
	{
		this->dp.regs[x] = this->regs[x];
		this->antidp.regs[x] = this->regs[x];
		this->p.regs[x] = this->regs[x];
	}
}

void loadNetworkIntoProgram(networkprogram &n)
{
	int REGcounter = 0;
	for(int i = 0; i<n.n.neurons.size(); ++i)
	{
		
		
		for(int x = 0; x < 6; ++x)
			n.n.neurons[i]->NX[x] = n.p.regs[REGcounter++];
		
		for(int x = 0; x<n.n.neurons[i]->outboundConnections.size(); ++x)
			 for(int z = 0; z<7; ++z)
				n.n.neurons[i]->outboundConnections[x]->SX[z] = n.p.regs[REGcounter++];
	}
}

void freenetworkprogram(networkprogram &np)
{
	for(int i = 0; i<np.regs.size(); ++i)
	{
		delete np.regs[i];
		np.regs[i] = NULL;
	}
	np.regs.clear();
	for(int i = 0; i<np.n.neurons.size(); ++i)
	{
		for(int x = 0; x<np.n.neurons[i]->outboundConnections.size(); ++x)
		{
			delete np.n.neurons[i]->outboundConnections[x];
			np.n.neurons[i]->outboundConnections[x] = NULL;
		}
		np.n.neurons[i]->outboundConnections.clear();
		
		delete np.n.neurons[i];
		np.n.neurons[i] = NULL;
		
	}
		
}

#endif
