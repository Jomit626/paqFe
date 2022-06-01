#pragma once

#include <cstring>
#include <cmath>
#include <assert.h>

#include "../types.hpp"
#include "StateMap.hpp"

#include "TabHash.hpp"
#include "TabHash.h"

namespace paqFe::internal {

#define MatchCxt uint16_t
#define MAXLEN 16      // FPGA对比最长16个字符匹配

template<int BUFSIZE>
class MatchModel {
protected:

    MatchCxt matchCxts[8]{0};

    bool first = true;
    bool bitCtxMatch[8] = {false};
    bool currByteMatch = true;
    int counter = 0;
    int binary_idx = 0;
    int matchPos = 0;
    int matchPos2 = 0;

    int expectedByte = 0;
    
    // bytewise context
    uint64_t C = 0;
    uint8_t C0 = 0; // current byte
    uint8_t C1 = 0;
    
    // match station
    int length = 0;

    uint32_t hashCtx = 0;
    uint32_t hashCtx2 = 0;

    uint8_t  buf[BUFSIZE];
    uint16_t hashTable[BUFSIZE / 4];
    static constexpr size_t bufMask = BUFSIZE - 1;
    static constexpr size_t hashMask = BUFSIZE / 4 - 1;
    uint64_t bufpos = 0;
    static_assert(isPow2(bufMask + 1));
    // TO DO：using "#define" to declare 8 Statemap
    StateMap< 1 << 5> sm1;
    StateMap< 1 << 6> sm2;
    StateMap< 1 << 7> sm3;
    StateMap< 1 << 8> sm4;
    StateMap< 1 << 9> sm5;
    StateMap< 1 << 10> sm6;
    StateMap< 1 << 11> sm7;
    StateMap< 1 << 12> sm8;


public:
    static constexpr int nProb = 1;
    static constexpr int nCtx = 1;
    MatchModel() : sm1(0, MAPTYPE::Run),sm2(1, MAPTYPE::Run),sm3(2, MAPTYPE::Run),
    sm4(3, MAPTYPE::Run),sm5(4, MAPTYPE::Run),sm6(5, MAPTYPE::Run),sm7(6, MAPTYPE::Run),
    sm8(7, MAPTYPE::Run){

    }

    void predict(uint8_t bit, Prob *pp, Context *pctx) {}

    void predict_byte(uint8_t byte, Prob *pp, Context *pctx, size_t pstride, size_t ctxstride) {
        // 如果length不为0则获取8个比特的上下文
        updateContext(byte);
        printf("preditct_byte info..........\n");
        if(length > 0){
            currByteMatch  = true;
            for(int bpos = 0; bpos < 8; bpos++){
                const int expectedBit = (length != 0) ? (expectedByte >> (7 - bpos)) & 1U : 0;
                
                if( (expectedByte >> (8 - bpos)) == (byte >> (8 - bpos))){
                    // matchCxt = 
                    matchCxts[bpos] = ((((length - 1) << 1) + expectedBit) << bpos) | (byte >> (8 - bpos));
                } else {
                    // 使用一个Order1 进行修正：
                    bitCtxMatch[bpos] = false;
                }
            }

            // TO DO: using "define " to call statemap's function
            sm1.predict(matchCxts[0],&pp[pstride * 0]);sm2.predict(matchCxts[1],&pp[pstride * 1]);sm3.predict(matchCxts[2],&pp[pstride * 2]);sm4.predict(matchCxts[3],&pp[pstride * 3]);
            sm5.predict(matchCxts[4],&pp[pstride * 4]);sm6.predict(matchCxts[5],&pp[pstride * 5]);sm7.predict(matchCxts[6],&pp[pstride * 6]);sm8.predict(matchCxts[7],&pp[pstride * 7]);

            sm1.update((byte >> 7) & 1);sm2.update((byte >> 6) & 1); sm3.update((byte >> 5) & 1);sm4.update((byte >> 4) & 1);
            sm5.update((byte >> 3) & 1);sm6.update((byte >> 2) & 1);sm7.update((byte >> 1) & 1);sm8.update((byte >> 0) & 1);

            for(int i = 0; i < 8; i++){
                currByteMatch &= bitCtxMatch[i];
            }
        } else {
            for(int i = 0; i < 8; i++){
                *(pp + i * pstride) = 0x800;
            }
        }
        setContext(pctx,ctxstride);
    }

    void updateContextSingleHash(uint8_t byte){
        // printf("updateContext info curr byte: %c ..........\n",byte);
        C = (C << 8) | byte;
        C1 = (C & 0xFF00) >> 8 ;
        // TO DO ：取前4个字节进行哈希处理 or 使用两个哈希表进行处理
        hashCtx = (hashCtx * (3 << 3) + C1) & hashMask;
        buf[bufpos] = byte;
        bufpos &= bufMask;

        if(length > 0 && currByteMatch){
            ++matchPos;
            matchPos &= bufMask;
            if(length < MAXLEN ) length++; 
        } else {
            length = 0;
            matchPos = hashTable[hashCtx];
            if(matchPos != bufpos){
                int i;
                while((length < MAXLEN ) && (((i = matchPos - length - 1) & bufMask) != bufpos) &&
                     buf[i] == buf[(bufpos - length - 1) & bufMask])
                    length += 1;
            }
            if(length > 2){
                expectedByte = buf[matchPos];
            }

        }
        // printf("length: %d matchPos: %d\n",length,matchPos);
        
        hashTable[hashCtx] = bufpos;
        bufpos++;
    }

    void updateContextDoubleHash(uint8_t byte){
        // printf("updateContext info curr byte: %c ..........\n",byte);
        C = (C << 8) | byte;
        C1 = (C & 0xFF00) >> 8 ;
        // TO DO ：取前4个字节进行哈希处理 or 使用两个哈希表进行处理
        hashCtx = (hashCtx * (3 << 3) + C1) & hashMask;
        hashCtx2 = (hashCtx2 * (5 << 5) + C1) & hashMask;
        buf[bufpos] = byte;
        bufpos &= bufMask;

        if(length > 0 && currByteMatch){
            ++matchPos;
            matchPos &= bufMask;
            if(length < MAXLEN ) length++; 
        } else {
            length = 0;
            int length1 = 0; int length2 = 0;
            matchPos = hashTable[hashCtx];
            if(matchPos != bufpos){
                int i;
                while((length1 < MAXLEN ) && (((i = matchPos - length1 - 1) & bufMask) != bufpos) &&
                     buf[i] == buf[(bufpos - length1 - 1) & bufMask])
                    length1 += 1;
            }
            
            matchPos2 = hashTable[hashCtx2];
            if(matchPos2 != bufpos){
                int j;
                while((length2 < MAXLEN ) && (((j = matchPos2 - length2 - 1) & bufMask) != bufpos) &&
                     buf[j] == buf[(bufpos - length2 - 1) & bufMask])
                    length2 += 1;
            }

            if(length1 > length2){
                length = length1;
                expectedByte = buf[matchPos];
            } else {
                length = length2;
                expectedByte = buf[matchPos2];
            }

        }
        // printf("length: %d matchPos: %d\n",length,matchPos);
        // 这里有点问题？
        
        hashTable[hashCtx] = bufpos;
        hashTable[hashCtx2] = bufpos;
        bufpos++;
    }

    void updateContext(uint8_t byte){
        // printf("updateContext info curr byte: %c ..........\n",byte);
        C = (C << 8) | byte;
        C1 = (C & 0xFF00) >> 8 ;
        // TO DO ：取前4个字节进行哈希处理 or 使用两个哈希表进行处理
        hashCtx = (hashCtx * (3 << 3) + C1) & hashMask;
        hashCtx2 = (hashCtx2 * (5 << 5) + C1) & hashMask;
        buf[bufpos] = byte;
        bufpos &= bufMask;

        if(length > 0 && currByteMatch){
            ++matchPos;
            matchPos &= bufMask;
            if(length < MAXLEN ) length++; 
        } else {
            length = 0;
            int length1 = 0; int length2 = 0;
            matchPos = hashTable[hashCtx];
            if(matchPos != bufpos){
                int i;
                while((length1 < MAXLEN ) && (((i = matchPos - length1 - 1) & bufMask) != bufpos) &&
                     buf[i] == buf[(bufpos - length1 - 1) & bufMask])
                    length1 += 1;
            }
            
            matchPos2 = hashTable[hashCtx2];
            if(matchPos2 != bufpos){
                int j;
                while((length2 < MAXLEN ) && (((j = matchPos2 - length2 - 1) & bufMask) != bufpos) &&
                     buf[j] == buf[(bufpos - length2 - 1) & bufMask])
                    length2 += 1;
            }

            if(length1 > length2){
                length = length1;
                expectedByte = buf[matchPos];
            } else {
                length = length2;
                expectedByte = buf[matchPos2];
            }

        }
        // printf("length: %d matchPos: %d\n",length,matchPos);
        
        hashTable[hashCtx] = bufpos;
        hashTable[hashCtx2] = bufpos;
        bufpos++;
    }

    void setContext(Context *pctx, size_t ctxstride){
        for(int i = 0; i < 8; i++){
            pctx[i * ctxstride] = (length>=4)+(length>=8)+(length>=12)+(length>=14);
        }
    }

};

using MatchModelDefault = MatchModel<1024 * 16>;

}