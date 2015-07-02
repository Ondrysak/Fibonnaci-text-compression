#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <stdint.h>
using namespace std;
#endif /* __PROGTEST__ */
//---------------------------
class bitworks
{
public:
    bitworks(const char *);
    bool addbit(bool);
    bool flush(void);
private:
    ofstream out;
    int cnt;
    char byte;
};
bitworks::bitworks(const char * name)
{
    out.open (name, ios::out|ios::binary|ios::app);
    cnt=0;
    byte=(char)0;
}
bool bitworks::addbit(bool val)
{
    if(val)
    {
        byte |= 1 << (cnt%8);
        cnt++;
    }
    else
    {
        cnt++;
    }
    if((cnt%8)==0)
    {
        out.put(byte);
        byte=(char)0;
        if(!out.good())
        {
            return false;
        }

    }
    return true;

}
bool bitworks::flush()
{
    if(byte==(char)0)
    {
        return true;
    }
    out.put(byte);
    if(!out.good())
    {
        return false;
    }
    return true;
}
//----------------------------
class bitworks_i
{
public:
    bitworks_i(const char *);
    int getbit(void);
    bool goodb(void);
private:
    ifstream in;
    int cnt;
    char byte;
};
bool bitworks_i::goodb(){
if(in.good()){
return true;
}
else{
return false;
}

}
bitworks_i::bitworks_i(const char * name)
{
    in.open (name, ios::in|ios::binary|ios::ate);
    in.seekg (0, ios::beg);
    cnt=0;
    byte=(char)0;
}
int bitworks_i::getbit(void)
{
    if(cnt%8==0)
    {
        byte=in.get();
        if(in.eof())
        {
            //-2 na EOF
            in.close();
            return -2;
        }
        if(!in.good()){
        return -1;
        }

    }
    if(byte & (1<<(cnt%8)))
    {
        cnt++;
        return 1;
    }
    else
    {
        cnt++;
        return 0;
    }

}


//----------------------------
/*
takes inFile in UTF8 encoding and compressed file in fibbonacci outFile is created
*/
bool Utf8ToFibonacci ( const char * inFile, const char * outFile )
{
    bitworks output(outFile);
    bool succ=true;
    int start;
    int ok;
    int fibo[29]= {1,2,3,5,8,13,21,34,55,89,144,233,377,610,987,1597,2584,4181,6765,10946,17711,28657,46368,75025,121393,196418,317811,514229,832040};
    int fibo_form[31];
    char  byte1,byte2,byte3,byte4;
    int val=0;
    int tempval=0;
    ifstream myfile;
    myfile.open (inFile, ios::in|ios::binary|ios::ate);
    if(!myfile.good())
    {
        return false;
    }
    myfile.seekg (0, ios::beg);
    if(!myfile.good())
    {
        return false;
    }
    //kazdy prubeh cyklu zpracuje jeden utf8 znak pokud tedy narazi na zacatek ktery by mel pokracovat dalsim bajtem nacte i ty a do val priradi prislusnou hodnotu;
    while(true)
    {
        ok = 0;
        byte1=myfile.get();
        if (myfile.eof())
        {
            break;
        }
        if(!myfile.good())
        {
            return false;
        }
        //0xxxxxxx 7bit
        if(!(byte1 & 0x80))
        {
            val=0;
            val= (int) byte1;
            ok=1;
        }
        //110xxxxx 10xxxxxx 5bit + 6bit
        if((byte1 & 0x80) && (byte1 & 0x40) && !(byte1 & 0x20))
        {
            val=0;
            val+=((byte1 & 0x1F)<<6);
            byte2=myfile.get();
            if (myfile.eof())
            {
                //neprisel mi druhy ocekavany byte
                return false;
                break;
            }
            if(!myfile.good())
            {
                return false;
            }
            if(!((byte2 & 0x80)&&!(byte2 & 0x40)))
            {
                return false;
            }
            val+=(byte2 & 0x3F);
            ok=1;
        }
        // 1110xxxx 10xxxxxx 10xxxxxx
        if((byte1 & 0x80) && (byte1 & 0x40) && (byte1 & 0x20) && !(byte1 & 0x10) )
        {
            val=0;
            val+=((byte1 & 0xF)<<12);
            byte2=myfile.get();
            if (myfile.eof())
            {
                //neprisel mi druhy ocekavany bit
                return false;
                break;
            }
            if(!myfile.good())
            {
                return false;
            }
            if(!((byte2 & 0x80)&&!(byte2 & 0x40)))
            {
                return false;
            }
            val+=((byte2 & 0x3F)<<6);
            byte3=myfile.get();
            if(!((byte3 & 0x80)&&!(byte3 & 0x40)))
            {
                return false;
            }
            if (myfile.eof())
            {
                //neprisel mi treti ocekavany bit
                return false;
                break;
            }
            if(!myfile.good())
            {
                return false;
            }
            val+=(byte3 & 0x3F);
            ok=1;
        }
        if((byte1 & 0x80) && (byte1 & 0x40) && (byte1 & 0x20) && (byte1 & 0x10) && !(byte1 & 0x8) )
        {
            val=0;
            val+=((byte1 & 0x7)<<18);
            byte2=myfile.get();
            if (myfile.eof())
            {
                //neprisel mi druhy ocekavany bit
                return false;
                break;
            }
            if(!myfile.good())
            {
                return false;
            }
            if(!((byte2 & 0x80)&&!(byte2 & 0x40)))
            {
                return false;
            }
            val+=((byte2 & 0x3F)<<12);
            byte3=myfile.get();
            if (myfile.eof())
            {
                //neprisel mi treti ocekavany bit
                return false;
                break;
            }
            if(!myfile.good())
            {
                return false;
            }
            if(!((byte3 & 0x80)&&!(byte3 & 0x40)))
            {
                return false;
            }
            val+=((byte3 & 0x3F)<<6);
            byte4=myfile.get();
            if (myfile.eof())
            {
                //neprisel mi ctvrty ocekavany bit
                return false;
                break;
            }
            if(!myfile.good())
            {
                return false;
            }
            if(!((byte4 & 0x80)&&!(byte4 & 0x40)))
            {
                return false;
            }
            val+=(byte4 & 0x3F);
            ok=1;
        }
        if(!ok)
        {
            return false;
        }
        //cout<<(bitset<8>)byte1<<endl;
        //cout<<"Hodnota:"<<val<<endl;
        //rozklad na fibo
        //posunu o 1 -> 0 se tedy bude kodovat jako 1
        tempval=val+1;
        start=0;
        //zjistim jake nejvetsi se mi tam vejde
        if(tempval>832040)
        {
            start=28;
        }
        if(tempval>1114112)
        {
            return false;
        }
        for(int j = 0; j<29; j++)
        {
            if(tempval<fibo[j])
            {
                start = j-1;
                break;
            }

        }
        //to jake nejvetsi se mi tam vejde take udava bitovou delku - 1 (ukoncujici 1) potrebnou k zapisu
        //zjistim z kterych to slozit
        // -> zacatek novyho cisla
        for(int j = 0; j<31; j++)
        {
            fibo_form[j]=0;
        }
        fibo_form[start+1]=1;
        for(int i=start; i>=0; i--)
        {
            if (tempval==0)
            {
                break;
            }
            if(tempval>=fibo[i])
            {
                //cout<<fibo[i]<<endl;
                fibo_form[i]=1;
                tempval-=fibo[i];

                i--;


            }


        }

        if (myfile.eof())
        {
            //neprisel mi druhy ocekavany bit
            return false;
            break;
        }
        for (int j = 0; j<30; j++)
        {
            //cout<<fibo_form[j];
            if(fibo_form[j]==1)
            {
                succ=output.addbit(true);
                if(!succ)
                {
                    return false;
                }

            }
            else
            {
                succ=output.addbit(false);
                if(!succ)
                {
                    return false;
                }
            }
            if(fibo_form[j]==1 && fibo_form[j-1]==1)
            {
                break;
            }
        }
        //cout<<endl;
    }

    succ=output.flush();
    if(!succ)
    {
        return false;
    }
    myfile.close();
    return true;
}
/*
takes inFile in fib encoding and decompressed file in UTF8 encoding soutFile is created
*/
bool FibonacciToUtf8 ( const char * inFile, const char * outFile )
{
    bool succ = true;
    bitworks_i input(inFile);
    succ = input.goodb();
    if(!succ){
    return false;
    }
    ofstream myfile;
    int bit,val,shifted;
    myfile.open (outFile, ios::out|ios::binary|ios::app);
    if(!myfile.good()){
    return false;
    }
    char byte1,byte2,byte3,byte4;
    int fibo[29]= {1,2,3,5,8,13,21,34,55,89,144,233,377,610,987,1597,2584,4181,6765,10946,17711,28657,46368,75025,121393,196418,317811,514229,832040};
    int fibo_form[31];
    while(true)
    {
        byte1=(char)0;
        byte2=byte1;
        byte3=byte1;
        byte4=byte1;
        for(int j = 0; j<31; j++)
        {
            fibo_form[j]=0;
        }
        for(int i = 0; i<31; i++)
        {
            // i uprostred znaku se muze cteni posrat

            bit=input.getbit();
            if(i==30 && bit != 1)
            {
                return false;
            }
            if(bit == -2)
            {
                break;
            }
            if(bit == -1){
            return false;
            }
            if(bit == 1)
            {
                fibo_form[i]=1;
            }
            if(i!=0 && fibo_form[i]==1 && fibo_form[i-1]==1)
            {
                break;
            }
        }
        if(bit == -2)
        {
            break;
        }
        val=0;
        for(int j = 0; j<31; j++)
        {

            //cout<<fibo_form[j];
            if(j!=0 && fibo_form[j]==1 && fibo_form[j-1]==1)
            {
                break;
            }
            if(fibo_form[j]==1)
            {
                val+=fibo[j];
            }
        }
        shifted=val-1;
        //cout <<endl<<shifted<< endl;
        if(shifted>1114111)
        {
            return false;
        }
        if(shifted>=0 && shifted<=127)
        {
            for (int i = 0; i<7; i++)
            {
                if(shifted & (1<<i))
                {
                    byte1 |= (1<<i);
                }

            }
            myfile.put(byte1);
            if(!myfile.good()){
            return false;
            }
        }
        if(shifted>=128 && shifted<=2047)
        {
            for (int i = 0; i<6; i++)
            {
                if(shifted & (1<<i))
                {
                    byte2 |= (1<<i);
                }

            }
            byte2 |= (1<<7);
            for (int i = 6; i<11; i++)
            {
                if(shifted & (1<<i))
                {
                    byte1 |= (1<<(i-6));
                }

            }
            byte1 |= (1<<6);
            byte1 |= (1<<7);
            myfile.put(byte1);
            myfile.put(byte2);
            if(!myfile.good()){
            return false;
            }
        }
        if(shifted>=2048 && shifted<=65535)
        {
            for (int i = 0; i<6; i++)
            {
                if(shifted & (1<<i))
                {
                    byte3 |= (1<<i);
                }

            }
            byte3 |= (1<<7);
            for (int i = 6; i<12; i++)
            {
                if(shifted & (1<<i))
                {
                    byte2 |= (1<<(i-6));
                }

            }
            byte2 |= (1<<7);
            for (int i = 12; i<16; i++)
            {
                if(shifted & (1<<i))
                {
                    byte1 |= (1<<(i-12));
                }

            }
            byte1 |= (1<<7);
            byte1 |= (1<<6);
            byte1 |= (1<<5);
            myfile.put(byte1);
            myfile.put(byte2);
            myfile.put(byte3);
            if(!myfile.good()){
            return false;
            }


        }
        if(shifted>=65536 && shifted<=1114111)
        {
            for (int i = 0; i<6; i++)
            {
                if(shifted & (1<<i))
                {
                    byte4 |= (1<<i);
                }

            }
            byte4 |= (1<<7);
            for (int i = 6; i<12; i++)
            {
                if(shifted & (1<<i))
                {
                    byte3 |= (1<<(i-6));
                }

            }
            byte3 |= (1<<7);
            for (int i = 12; i<18; i++)
            {
                if(shifted & (1<<i))
                {
                    byte2 |= (1<<(i-12));
                }

            }
            byte2 |= (1<<7);
            for (int i = 18; i<21; i++)
            {
                if(shifted & (1<<i))
                {
                    byte1 |= (1<<(i-18));
                }

            }


            byte1 |= (1<<7);
            byte1 |= (1<<6);
            byte1 |= (1<<5);
            byte1 |= (1<<4);
            myfile.put(byte1);
            myfile.put(byte2);
            myfile.put(byte3);
            myfile.put(byte4);
            if(!myfile.good()){
            return false;
            }
        }

    }
    myfile.close();
    return true;
}

#ifndef __PROGTEST__
int main ( int argc, char * argv [] )
{
    return 0;
}
#endif /* __PROGTEST__ */
