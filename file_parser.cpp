#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <math.h>

using namespace std;
vector <vector <vector <unsigned int> > > diffTable;

const uint32_t diffTabOriginal[16][16]={                            // best     ave(exp)    worst
{16 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0},   // 1.0      1.0         1.0
{0 , 0 , 0 , 0 , 0 , 2 , 2 , 0 , 0 , 0 , 4 , 0 , 0 , 2 , 2 , 4 },   // 0.25     0.16667     0.125
{0 , 0 , 0 , 4 , 0 , 4 , 0 , 0 , 0 , 0 , 0 , 4 , 0 , 4 , 0 , 0 },   // 0.25     0.25        0.25
{0 , 0 , 4 , 0 , 0 , 2 , 2 , 0 , 0 , 0 , 0 , 0 , 0 , 2 , 2 , 4 },   // 0.25     0.16667     0.125
{0 , 0 , 0 , 2 , 0 , 2 , 0 , 4 , 0 , 4 , 0 , 2 , 0 , 2 , 0 , 0 },   // 0.25     0.16667     0.125
{0 , 2 , 0 , 2 , 0 , 0 , 2 , 2 , 2 , 0 , 2 , 0 , 2 , 2 , 0 , 0 },   // 0.125    0.125       0.125
{0 , 0 , 2 , 0 , 2 , 0 , 0 , 0 , 4 , 0 , 2 , 0 , 2 , 0 , 4 , 0 },   // 0.25     0.16667     0.125
{0 , 2 , 2 , 0 , 2 , 2 , 2 , 2 , 2 , 0 , 0 , 2 , 0 , 0 , 0 , 0 },   // 0.125    0.125       0.125
{0 , 0 , 0 , 2 , 0 , 0 , 2 , 0 , 0 , 0 , 0 , 2 , 4 , 0 , 2 , 4 },   // 0.25     0.16667     0.125
{0 , 4 , 2 , 0 , 2 , 0 , 0 , 0 , 0 , 4 , 2 , 0 , 2 , 0 , 0 , 0 },   // 0.25     0.16667     0.125
{0 , 0 , 0 , 2 , 4 , 0 , 2 , 0 , 0 , 0 , 0 , 2 , 0 , 0 , 2 , 4 },   // 0.25     0.16667     0.125
{0 , 4 , 2 , 0 , 2 , 0 , 0 , 0 , 0 , 4 , 2 , 0 , 2 , 0 , 0 , 0 },   // 0.25     0.16667     0.125
{0 , 2 , 0 , 0 , 0 , 2 , 2 , 2 , 2 , 0 , 2 , 2 , 2 , 0 , 0 , 0 },   // 0.125    0.125       0.125
{0 , 0 , 2 , 2 , 2 , 2 , 0 , 0 , 0 , 0 , 2 , 2 , 2 , 2 , 0 , 0 },   // 0.125    0.125       0.125
{0 , 2 , 2 , 2 , 2 , 0 , 2 , 2 , 2 , 0 , 0 , 0 , 0 , 2 , 0 , 0 },   // 0.125    0.125       0.125
{0 , 0 , 0 , 0 , 0 , 0 , 0 , 4 , 4 , 4 , 0 , 0 , 0 , 0 , 4 , 0 },   // 0.25     0.25        0.25
};

int hex2dec(string hex_val)
{
    if (hex_val == "0x0") return 0;
    else if (hex_val==("0x1")) return 1;
    else if (hex_val==("0x2")) return 2;
    else if (hex_val==("0x3")) return 3;
    else if (hex_val==("0x4")) return 4;
    else if (hex_val==("0x5")) return 5;
    else if (hex_val == "0x6") return 6;
    else if (hex_val==("0x7")) return 7;
    else if (hex_val==("0x8")) return 8;
    else if (hex_val==("0x9")) return 9;
    else if (hex_val==("0xA")) return 10;
    else if (hex_val==("0xB")) return 11;
    else if (hex_val==("0xC")) return 12;
    else if (hex_val==("0xD")) return 13;
    else if (hex_val==("0xE")) return 14;
    else if (hex_val==("0xF")) return 15;
}

int main()
{



    ifstream file("7r-52-0.txt");
    int round = 7;

    auto differential = new int[round+1][16];
    double probability[round] = {0};


    string str;
    string check_before = "before_sbox_difference";
    string check_after = "after_sbox_difference";
    vector<string> store_before;
    vector<string> store_after;
    unsigned int counter_b = 0;
    unsigned int counter_a = 0;

    while (std::getline(file, str)) {
        if (str.find(check_before)!=string::npos)
        {
            store_before.push_back(str);
            //cout << store_before[counter_b] << endl;
            counter_b++;
        }
        if (str.find(check_after)!=string::npos)
        {
            store_after.push_back(str);
            //cout << store_after[counter_a] << endl;
            counter_a++;
        }
    }

    //cout << endl;
    int start, end;
    string temp;
    int before_sbox, after_sbox;
    double prob=1, round_prob=1, overall_prob=1;
    for (int r=0; r<=round; r++)
    {
        round_prob=1;
        //cout << "Round:" << r << endl;
        for (int s=0; s<16; s++)
        {
            prob=1;
            check_before = "before_sbox_difference_" + to_string(r) + "_" + to_string(s)+ " ";
            check_after = "after_sbox_difference_" + to_string(r) + "_" + to_string(s)+ " ";

            for(vector <string> :: iterator it = store_before.begin(); it != store_before.end(); ++it){
                str = *it;
                if (str.find(check_before)!=string::npos)
                {
                    //cout << str << endl;
                    start = str.find("0x");
                    temp = str.substr(start);
                    end = temp.find(" ");
                    temp = temp.substr(0,end);
                    before_sbox = hex2dec(temp);
                    //cout << dec << temp << " " << r << " " << s << " " << hex << before_sbox << endl;
                    differential[r][s] = before_sbox;
                    break;
                }
            }
            for(vector <string> :: iterator it = store_after.begin(); it != store_after.end(); ++it){
                str = *it;
                if (str.find(check_after)!=string::npos)
                {
                    //cout << str << endl;
                    start = str.find("0x");
                    temp = str.substr(start);
                    end = temp.find(" ");
                    temp = temp.substr(0,end);
                    after_sbox = hex2dec(temp);
                    //cout << dec << temp << " " << after_sbox << endl;
                    break;
                }
            }
            if (r!=round)
            {
                prob = diffTabOriginal[before_sbox][after_sbox]/16.0;
                //cout << "Sbox Probability: " << prob << endl;
                round_prob*=prob;
                //cout << "Round Probability: " << round_prob << endl;
                overall_prob*=prob;
                //cout << endl;
            }


        }
        //cout << "Total Round Probability = " << round_prob << endl;
        probability[r] = round_prob;
        //cout << "Current Overall Probability = " << overall_prob << endl;
        //getchar();
    }

        cout << "Overall Probability = " << log2(overall_prob) << endl;
        for (int r=0; r<=round; r++)
        {
            cout << "Round = " << r << ": ";
            for (int s=15; s>=0; s--)
            {
                cout << hex << differential[r][s];
            }
            cout << " " << log2(probability[r]) << endl;
        }

        cout << "For clustering" << endl;

        cout << "\t{ " << endl;
        for (int r=0; r<=round; r++)
        {
            //cout << "Round = " << r << endl;
            cout << "\t\t{ ";
            for (int s=0; s<16; s++)
                cout << hex << "0x"<< differential[r][s] << ", ";
            cout << " }," << endl;;
        }
        cout << "\t}," << endl;;

//    std::size_t found = str.find(check_before);
//    if (found!=string::npos)
//    cout << str << endl;

}
