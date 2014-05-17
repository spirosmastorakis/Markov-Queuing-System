#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

int counter_convergence=0,count=0 , arrivals_sum=0 , state=0 , K ,N, i, MAX_LIM ;
int *arrival;
int *arrival_test;
float average=0,temp_average=0,eps ;
float *prob;
float *test_array;
int flag_convergence=0 ;
int flag_flush=0 ;

//convergence check
void check_convergence() {
	counter_convergence=0;
	temp_average=0;
	for(i=0;i<=N;i++){
    	//transition probability computation
    	if(i<K){
        	prob[i]=1.0*arrival[i]/arrivals_sum;
            test_array[i]=1.0*arrival_test[i]/arrivals_sum;
            prob[i] = prob[i]+test_array[i];
        }
        else{
    		prob[i]=1.0*arrival[i]/arrivals_sum;
    	}
        temp_average = temp_average + i* prob[i];                      
	}
	if ((fabs(temp_average-average)) < eps )
	// | E(n) - E'(n)| < eps
			flag_convergence=1;
    	average = temp_average;
	return;
}  

void packet_arrival(){
     	if (state < K){
     	//increase arrivals
        //perform state transition
      		arrivals_sum = arrivals_sum + 1 ;
      		arrival_test[state]= arrival_test[state] + 1;
      	}
     	else{
     	//increase arrivals
        //perform state transition
      		arrivals_sum = arrivals_sum + 1 ;
      		arrival[state]= arrival[state] + 1;
      	}
     	count++;
     	counter_convergence++;
     	//if the number of enqueued packets reaches
     	//K, then output traffic
     	if (K==0 && state==K){
        	state++;
    		flag_flush=1;
        	return;
	}
     	if (state < N)
        	state++;
     	if ((state-1)==K)
        	flag_flush= 1;
     	return;
}

void packet_departure(){
	//perform state transition
    	count= count + 1 ;
     	counter_convergence++;
     	state= state - 1;
     	//if the number of packets reaches 0
     	//then enqueue incoming packets and
        //stop traffic output
     	if (state==0)
        	flag_flush=0;
     	return;
}

int main(int argc, char* argv[]){

	int verbose;	
	float random_number, total_mean_time=0, Pblocking=0, throughput=0, l , m;

	srand(time(NULL));

	//parse input parameters
    	if (argc != 8) {
    		printf("Unexpected arguments. Please try again.\n");
		return 1;
    	}
	
    	N = atoi(argv[1]); //system capacity
    	K = atoi(argv[2]); //threshold
    	l = atof(argv[3]); //arrival rate
    	m = atof(argv[4]); //service rate
    	MAX_LIM = atoi(argv[5]); //maximum number of iterations
    	eps = atof(argv[6]); //eps
	verbose= atoi(argv[7]); //verbose output or not?
 
    	//allocate memory  
    	arrival=(int *) malloc((N+1)*sizeof(int));
    	arrival_test=(int *) malloc((N+1)*sizeof(int));
    	prob=(float *) malloc((N+1)*sizeof(float));
    	test_array=(float *) malloc((N+1)*sizeof(float));

    	for (i=0; i<=N; i++) {
    		arrival[i]=0;
        	prob[i]=1.0*0;
        	test_array[i]=1.0*0;
        	arrival_test[i]=0;
        }   
    	while ((count < MAX_LIM) && !(flag_convergence)){
        	if (state == 0) {
            	packet_arrival();
            	continue;
          	}
          	if (!flag_flush){
            		packet_arrival();
            		//convergence check every 1000 iterations
              		if (counter_convergence == 1000)
            	 		check_convergence();
            		continue;
             	}
             	// 0 < random number < 1
          	random_number = (rand() / (RAND_MAX*1.0)) ;
          	if (random_number < (l/(1.0*(l+m)))){
          		//packet arrival
           		packet_arrival();
            		//convergence check every 1000 iterations
        	  	if (counter_convergence == 1000)
            			check_convergence();
        	 	continue;
             	}
          	else {
          		//packet departure
           		 packet_departure();
            		//convergence check every 1000 iterations
            		if (counter_convergence == 1000)
            			check_convergence();
            		continue;
              	}
    	}
    	average = 0 ;

	printf("STATISTICS\n");
	printf("==============\n");
	printf("system capacity: %d packets\n",N);
	printf("buffering threshold: %d packets\n",K);
	printf("arrival rate: %f packets/sec\n",l);
	printf("departure rate: %f packets/sec\n",m);
	printf("maximum number of iterations: %d\n",MAX_LIM);
	printf("eps:%f\n",eps);

	for(i=0;i<=N;i++){ //transition probability computation
    		if(i<K){
        		prob[i]=1.0*arrival[i]/arrivals_sum;
            		test_array[i]=1.0*arrival_test[i]/arrivals_sum;
            		prob[i] = prob[i]+test_array[i];
        	}
        	else
        		prob[i]=1.0*arrival[i]/arrivals_sum;
		if (verbose)
 			printf("P[%d]=%f\n",i,prob[i]);
        average = average + i* prob[i];
	}   
                 
    	printf("Average queue size: %f packets\n",average);
    	//Little's law
    	total_mean_time = (average / (1.0*l));
    	printf("Average sojourn time: %f sec\n",total_mean_time);
    	Pblocking = prob[N];
    	printf("Blocking probability: %f \n",Pblocking);
    	throughput = (1.0*l)*(1-Pblocking);
    	printf("Throughput: %f packets/sec\n",throughput);
    	printf("The simulation was terminated after %d iterations\n\n",count);

    	free(arrival);
    	free(arrival_test);
    	free(prob);
    	free(test_array);
    	return 0;                
}            

    
