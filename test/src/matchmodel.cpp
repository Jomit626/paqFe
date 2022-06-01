#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>
#include <cstring>
#include <paqFe/models/MatchModel.hpp> 
#include <paqFe/types.hpp>

int main(){
    printf("testing MatchModel......\n");
    paqFe::internal::MatchModel<1024> mm;
    FILE* fp = fopen("../test/src/test.txt","rw");
    uint8_t buf[1024];
    int n = fread(buf,1,1024,fp);

    uint32_t pp[8 * 2]{0};
    uint32_t pctx[8 * 2];
    for(int i = 0; i < n; i++){
        mm.predict_byte(buf[i],pp,pctx,2,2);
        for(int j = 0; j < 8 * 2; j++){
            printf("%d ",pp[j]);
        }
        printf("\n\n\n\n");
        std::memset(pp,0,16);
    }
    
    printf("read %d byte\n",n);
    
    fclose(fp);

}