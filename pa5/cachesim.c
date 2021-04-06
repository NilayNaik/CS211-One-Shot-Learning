/* CS 211 PA4
 * Created for: nsn29
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

int miss,hit,memRead,memWrite;
struct cacheLine** cache;
unsigned long int count;

typedef struct cacheLine{

    unsigned long int tag;
    int valid;
    unsigned long int time;

}cacheLine;

cacheLine** createcache(int setnum,int assoc){
    int i,j;
    cache=(cacheLine**)malloc(setnum*sizeof(cacheLine*));
    for(i=0;i<setnum;i++){
        cache[i]=(cacheLine*)malloc((assoc)*sizeof(cacheLine));
    }

    for(i=0;i<setnum;i++){
        for(j=0;j<assoc;j++){
            cache[i][j].valid=0;
        }
    }

    return cache;
}


void readsim(unsigned long int tagindex,unsigned long int setindex,int assoc){

    int i,j,min;

    for(i=0;i<assoc;i++){
	    if(cache[setindex][i].valid==0){
	        miss++;
	        memRead++;
	        count++;
	        cache[setindex][i].valid=1;
	        cache[setindex][i].tag=tagindex;
	        cache[setindex][i].time=count;	
	        return;
	    }else {		
		    if(cache[setindex][i].tag==tagindex){
		    	hit++;
		    	return;
		    }		
		    if(i==(assoc-1)){
		    	miss++;
		    	memRead++;		
		    	min=0;
		    	for(j=0;j<assoc;j++){		
			        if(cache[setindex][j].time<=cache[setindex][min].time){
				        min=j;
			        }	
			    }	
			    cache[setindex][min].valid=1;
	            cache[setindex][min].tag=tagindex;
	            count++;
	            cache[setindex][min].time=count;
	            return;
		    }
		}	
	}

    return;

}

void writesim(unsigned long int tagindex,unsigned long int setindex,int assoc){

    int i,j,min;

    for(i=0;i<assoc;i++){
	    if(cache[setindex][i].valid==0){
	        miss++;
	        memRead++;
	        memWrite++;
	        count++;
	        cache[setindex][i].valid=1;
	        cache[setindex][i].tag=tagindex;
	        cache[setindex][i].time=count;
	        return;
	    }else {	
		    if(cache[setindex][i].tag==tagindex){
			    hit++;
			    memWrite++;
			    return;
		    }		
		    if(i==(assoc-1)){
			    miss++;
			    memRead++;
			    memWrite++;
			    min=0;
			    for(j=0;j<assoc;j++){	
			        if(cache[setindex][j].time<=cache[setindex][min].time){
				        min=j;
			        }
			    }
			
			    cache[setindex][min].valid=1;
	            cache[setindex][min].tag=tagindex;
	            count++;
	            cache[setindex][min].time=count;
	            return;
		    }
		}
	}

    return;
}

void clear(int setnum, int assoc){
	int i,j;
	for(i=0;i<setnum;i++){
		for(j=0;j<assoc;j++){
			cache[i][j].tag=0;
			cache[i][j].valid=0;
			cache[i][j].time=0;
	    }
	}
	miss=0;
	hit=0;
	memRead=0;
	memWrite=0;
	count=0;
    return;
}

void prefetchww(unsigned long int tagindex,unsigned long int setindex,int assoc){
	
	int i,j,min;

    for(i=0;i<assoc;i++){
	    if(cache[setindex][i].valid==0){
	        memRead++;
	        count++;
	        cache[setindex][i].valid=1;
	        cache[setindex][i].tag=tagindex;
	        cache[setindex][i].time=count;
	        return;
	    }else {		
		    if(cache[setindex][i].tag==tagindex){
			    return;
		    }	
		    if(i==(assoc-1)){
			    memRead++;
			    min=0;
			    for(j=0;j<assoc;j++){
			        if(cache[setindex][j].time<=cache[setindex][min].time){
				        min=j;
			        }	
			    }

			    cache[setindex][min].valid=1;
	            cache[setindex][min].tag=tagindex;
	            count++;
	            cache[setindex][min].time=count;
	            return;
		    }
		}
	}

    return;
}

void prefetchrr(unsigned long int tagindex,unsigned long int setindex,int assoc){
	
	int i,j,min;

    for(i=0;i<assoc;i++){
	    if(cache[setindex][i].valid==0){
	        memRead++;
	        count++;
	        cache[setindex][i].valid=1;
	        cache[setindex][i].tag=tagindex;
	        cache[setindex][i].time=count;
	        return;
	    }else {
		    if(cache[setindex][i].tag==tagindex){
			    return;
		    }
		    if(i==(assoc-1)){	
			    memRead++;	
			    min=0;
			    for(j=0;j<assoc;j++){
			        if(cache[setindex][j].time<=cache[setindex][min].time){
				        min=j;
			        }	
			    }

			    cache[setindex][min].valid=1;
	            cache[setindex][min].tag=tagindex;
	            count++;
	            cache[setindex][min].time=count;
	            return;
		    }
		}
	}


	return;
}
	
void prefetchw(unsigned long int tagindex,unsigned long int setindex,int assoc,unsigned long int tagindexn,unsigned long int setindexn){
	
    int i,j,min;

    for(i=0;i<assoc;i++){
	    if(cache[setindex][i].valid==0){
	        miss++;
	        memRead++;
	        memWrite++;
	        count++;
	        cache[setindex][i].valid=1;
	        cache[setindex][i].tag=tagindex;
	        cache[setindex][i].time=count;
	        prefetchww(tagindexn,setindexn,assoc);
	        return;
	    }else {
		    if(cache[setindex][i].tag==tagindex){
			    hit++;
			    memWrite++;
			    return;
		    }
		    if(i==(assoc-1)){
			    miss++;
			    memRead++;
			    memWrite++;
			    min=0;
			    for(j=0;j<assoc;j++){
			        if(cache[setindex][j].time<=cache[setindex][min].time){
				        min=j;
			        }	
			    }
			    cache[setindex][min].valid=1;
	            cache[setindex][min].tag=tagindex;
	            count++;
	            cache[setindex][min].time=count;
	            prefetchww(tagindexn, setindexn,assoc);
	            return;
		    }
		}
	}

    return;
}

void prefetchr(unsigned long int tagindex,unsigned long int setindex,int assoc,unsigned long int tagindexn,unsigned long int setindexn){
	
    int i,j,min;

    for(i=0;i<assoc;i++){
	    if(cache[setindex][i].valid==0){
	        miss++;
	        memRead++;
	        count++;
	        cache[setindex][i].valid=1;
	        cache[setindex][i].tag=tagindex;
	        cache[setindex][i].time=count;
	        prefetchrr(tagindexn,setindexn,assoc);
	        return;
	    }else {		
		    if(cache[setindex][i].tag==tagindex){
			    hit++;
			    return;
		    }	
		    if(i==(assoc-1)){
			    miss++;
			    memRead++;
			    min=0;
			    for(j=0;j<assoc;j++){
			        if(cache[setindex][j].time<=cache[setindex][min].time){
			        	min=j;
			        }	
			    }
			    cache[setindex][min].valid=1;
	            cache[setindex][min].tag=tagindex;
	            count++;
	            cache[setindex][min].time=count;
	            prefetchrr(tagindexn,setindexn,assoc);
	            return;
		    }
		}
	}

    return;
	
}

void readsiml(unsigned long int tagindex,unsigned long int setindex,int assoc){

    int i,j,min;

    for(i=0;i<assoc;i++){
	    if(cache[setindex][i].valid==0){
	        miss++;
	        memRead++;
	        count++;
	        cache[setindex][i].valid=1;
	        cache[setindex][i].tag=tagindex;
	        cache[setindex][i].time=count;
	        return;
	    }else {
		    if(cache[setindex][i].tag==tagindex){
			    hit++;
			    count++;
			    cache[setindex][i].time=count;
			    return;
		    }		
		    if(i==(assoc-1)){
		    	miss++;
		    	memRead++;
		    	min=0;
		    	for(j=0;j<assoc;j++){
		    	    if(cache[setindex][j].time<=cache[setindex][min].time){
			            	min=j;
			        }	
			    }
		    	cache[setindex][min].valid=1;
	            cache[setindex][min].tag=tagindex;
	            count++;
	            cache[setindex][min].time=count;
	            return;
		    }
		}
	}

    return;
}

void writesiml(unsigned long int tagindex,unsigned long int setindex,int assoc){

    int i,j,min;

    for(i=0;i<assoc;i++){
	    if(cache[setindex][i].valid==0){
	        miss++;
	        memRead++;
	        memWrite++;
	        count++;
	        cache[setindex][i].valid=1;
	        cache[setindex][i].tag=tagindex;
	        cache[setindex][i].time=count;
	
	        return;
	    }else {

		    if(cache[setindex][i].tag==tagindex){
		    	hit++;
		    	memWrite++;
		    	count++;
		    	cache[setindex][i].time=count;
		    	return;
		    }
			
		    if(i==(assoc-1)){
		    	miss++;
		    	memRead++;
		    	memWrite++;
		    	min=0;
		    	for(j=0;j<assoc;j++){
		        	if(cache[setindex][j].time<=cache[setindex][min].time){
		        		min=j;
		        	}	
		    	}			
			    cache[setindex][min].valid=1;
	            cache[setindex][min].tag=tagindex;
	            count++;
	            cache[setindex][min].time=count;
	            return;
		    }
		}
	}

    return;
}

void prefetchwwl(unsigned long int tagindex,unsigned long int setindex,int assoc){
	
	int i,j,min;

    for(i=0;i<assoc;i++){
	    if(cache[setindex][i].valid==0){
	        memRead++;
	        count++;
	        cache[setindex][i].valid=1;
	        cache[setindex][i].tag=tagindex;
	        cache[setindex][i].time=count;
	        return;
	    }else {
		    if(cache[setindex][i].tag==tagindex){
			    return;
		    }
		    if(i==(assoc-1)){
			    memRead++;
			    min=0;
			    for(j=0;j<assoc;j++){
			        if(cache[setindex][j].time<=cache[setindex][min].time){
			    	    min=j;
			        }	
			    }
			    cache[setindex][min].valid=1;
	            cache[setindex][min].tag=tagindex;
	            count++;
	            cache[setindex][min].time=count;
	            return;
		    }
		}
	}

    return;
}

void prefetchrrl(unsigned long int tagindex,unsigned long int setindex,int assoc){
	
	int i,j,min;

    for(i=0;i<assoc;i++){
	    if(cache[setindex][i].valid==0){
	        memRead++;
	        count++;
	        cache[setindex][i].valid=1;
	        cache[setindex][i].tag=tagindex;
	        cache[setindex][i].time=count;
	        return;
	    }else {
		    if(cache[setindex][i].tag==tagindex){
			    return;
		    }		
		    if(i==(assoc-1)){
		    	memRead++;	
		    	min=0;
		    	for(j=0;j<assoc;j++){
		    	    if(cache[setindex][j].time<=cache[setindex][min].time){
		    	    	min=j;
		    	    }	
		    	}
		    	cache[setindex][min].valid=1;
	            cache[setindex][min].tag=tagindex;
	            count++;
	            cache[setindex][min].time=count;
	            return;
		    }
	    }	
	}
	
    return;
}
	
void prefetchwl(unsigned long int tagindex,unsigned long int setindex,int assoc,unsigned long int tagindexn,unsigned long int setindexn){
	
    int i,j,min;

    for(i=0;i<assoc;i++){
	    if(cache[setindex][i].valid==0){
	        miss++;
	        memRead++;
	        memWrite++;
	        count++;
	        cache[setindex][i].valid=1;
	        cache[setindex][i].tag=tagindex;
	        cache[setindex][i].time=count;
	        prefetchww(tagindexn,setindexn,assoc);
	        return;
	    }else {
		    if(cache[setindex][i].tag==tagindex){
			    hit++;
			    memWrite++;
			    count++;
			    cache[setindex][i].time=count;
			    return;
		    }
		    if(i==(assoc-1)){
			    miss++;
			    memRead++;
			    memWrite++;
			    min=0;
			    for(j=0;j<assoc;j++){
			        if(cache[setindex][j].time<=cache[setindex][min].time){
				        min=j;
			        }	
			    }
			    cache[setindex][min].valid=1;
	            cache[setindex][min].tag=tagindex;
	            count++;
	            cache[setindex][min].time=count;
	            prefetchww(tagindexn, setindexn,assoc);
	            return;
		    }
		}
	}
    return;
}

void prefetchrl(unsigned long int tagindex,unsigned long int setindex,int assoc,unsigned long int tagindexn,unsigned long int setindexn){
	
    int i,j,min;

    for(i=0;i<assoc;i++){
	    if(cache[setindex][i].valid==0){
	        miss++;
	        memRead++;
	        count++;
	        cache[setindex][i].valid=1;
            cache[setindex][i].tag=tagindex;
        	cache[setindex][i].time=count;
	
        	prefetchrr(tagindexn,setindexn,assoc);
	        return;
	    }else {
		    if(cache[setindex][i].tag==tagindex){
			    hit++;
			    count++;
			    cache[setindex][i].time=count;
			    return;
		    }	
		    if(i==(assoc-1)){
		    	miss++;
			    memRead++;
			    min=0;
			    for(j=0;j<assoc;j++){
			        if(cache[setindex][j].time<=cache[setindex][min].time){
			        	min=j;
			        }	
			    }	
			    cache[setindex][min].valid=1;
	            cache[setindex][min].tag=tagindex;
	            count++;
	            cache[setindex][min].time=count;
	            prefetchrr(tagindexn,setindexn,assoc);
	            return;
		    }

		}
	}

    return;
}

int main(int argc, char** argv){

    int cachesize=atoi(argv[1]);
    int blocksize=atoi(argv[4]);
    int nAssoc;
    int setnum;
    int assoc;

    int b;
    int s;

    char work;
    unsigned long int address,newaddress,setmask,tagindex,setindex,tagindexn,setindexn;

    if(argv[3][0]=='f'){

        FILE* fl;
        fl=fopen(argv[5],"r");

        if(argv[2][0]=='d'){
            assoc=1;
            setnum=cachesize/blocksize;
        }else if (argv[2][5]!=':'){
            setnum=1;
            assoc=cachesize/blocksize;
        }else{
            sscanf(argv[2],"assoc:%d",&nAssoc);
            assoc=nAssoc;
            setnum=cachesize/blocksize/nAssoc;
        }

        b=log(blocksize)/log(2);
        s=log(setnum)/log(2);

        setmask=((1<<s)-1);

        cache=createcache(setnum,assoc);

        while(fscanf(fl, "%*x: %c %lx", &work, &address)==2){

            setindex=(address>>b)&setmask;
            tagindex=address>>(b+s);

            if(work=='R'){
	            readsim(tagindex,setindex,assoc);
	        }else if (work=='W'){
		        writesim(tagindex,setindex,assoc);
	        }
        }

        fclose(fl);

        fl=fopen(argv[5],"r");


        printf("Prefetch 0\n");
        printf("Memory reads: %d\nMemory writes: %d\nCache hits: %d\nCache misses: %d\n",memRead,memWrite,hit,miss);

        clear(setnum,assoc);

        while(fscanf(fl, "%*x: %c %lx", &work, &address)==2){

            setindex=(address>>b)&setmask;
            tagindex=address>>(b+s);

            newaddress=address+blocksize;
            setindexn=(newaddress>>b)&setmask;
            tagindexn=newaddress>>(b+s);

            if(work=='R'){
	            prefetchr(tagindex,setindex,assoc,tagindexn,setindexn);
	        }else if(work=='W'){
	        	prefetchw(tagindex,setindex,assoc,tagindexn,setindexn);
	        }
        }

        printf("Prefetch 1\n");
        printf("Memory reads: %d\nMemory writes: %d\nCache hits: %d\nCache misses: %d\n",memRead,memWrite,hit,miss);
 

    }else if (argv[3][0]=='l'){ 
	
        FILE* fl;
        fl=fopen(argv[5],"r");

        if(argv[2][0]=='d'){

            assoc=1;
            setnum=cachesize/blocksize;

        }else if (argv[2][5]!=':'){
            setnum=1;
            assoc=cachesize/blocksize;
        }else {

            sscanf(argv[2],"assoc:%d",&nAssoc);
            assoc=nAssoc;
            setnum=cachesize/blocksize/nAssoc;

        }

        b=log(blocksize)/log(2);
        s=log(setnum)/log(2);

        setmask=((1<<s)-1);

        cache=createcache(setnum,assoc);

        while(fscanf(fl, "%*x: %c %lx", &work, &address)==2){

            setindex=(address>>b)&setmask;
            tagindex=address>>(b+s);

            if(work=='R'){
	
	            readsiml(tagindex,setindex,assoc);
	        }else if (work=='W'){
		        writesiml(tagindex,setindex,assoc);
	        }
        }

        fclose(fl);
        fl=fopen(argv[5],"r");

        printf("Prefetch 0\n");
        printf("Memory reads: %d\nMemory writes: %d\nCache hits: %d\nCache misses: %d\n",memRead,memWrite,hit,miss);

        clear(setnum,assoc);

        while(fscanf(fl, "%*x: %c %lx", &work, &address)==2){

            setindex=(address>>b)&setmask;
            tagindex=address>>(b+s);

            newaddress=address+blocksize;
            setindexn=(newaddress>>b)&setmask;
            tagindexn=newaddress>>(b+s);

            if(work=='R'){
	            prefetchrl(tagindex,setindex,assoc,tagindexn,setindexn);
	        }else if(work=='W'){
		        prefetchwl(tagindex,setindex,assoc,tagindexn,setindexn);
	        }
        }

        printf("Prefetch 1\n");
        printf("Memory reads: %d\nMemory writes: %d\nCache hits: %d\nCache misses: %d\n",memRead,memWrite,hit,miss);
	
	} else{
		printf("Policy error");
	}

    return EXIT_SUCCESS;
}