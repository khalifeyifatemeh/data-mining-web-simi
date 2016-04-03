#include<iostream>
#include<string>
#include<stdlib.h>
#include<fstream>
#include<vector>
#include<iomanip>
#include<stdio.h>
#include<regex>
#include<algorithm>
#include<sstream>
#include<map>
#include<math.h>
#include <curl/curl.h>
using namespace std;
string source;

size_t writeCallback(char* buf, size_t size, size_t nmemb){
    for(size_t c= 0; c<size*nmemb;c++)
    {
        source.push_back(buf[c]);
    }
    return size*nmemb;
}
//give source code of website
void getsource(const string& s, string ss)
{

    CURL* url;
    curl_global_init(CURL_GLOBAL_ALL);
    url = curl_easy_init();

    curl_easy_setopt(url, CURLOPT_URL, s.c_str());
    curl_easy_setopt(url, CURLOPT_WRITEFUNCTION, &writeCallback);
    curl_easy_setopt(url, CURLOPT_VERBOSE, 1L);
    curl_easy_perform(url);

    ofstream file (ss.c_str());
    file << source;
    file.close();

    curl_easy_cleanup(url);
    curl_global_cleanup();
    source="";
}
//for split texts of sourcecode
void split(string & s, char c, map<string,int> & v)
{
    size_t p = 0;
    size_t q = s.find(c);

    while (q != string::npos) {
        v[s.substr(p, q - p)]++;
        p = ++q;
        q = s.find(c, q);

        if (q == string::npos)
            v[s.substr(p, s.length())]++;
    }
}

int main()
{
    string webAddress1;
    string webAddress2;
    string szFileName1 = "result1.txt";
    string szFileName2 = "result2.txt";

    cout << "Please enter first web address: ";	 // example: http://supercomputingblog.com
//    cin >> webAddress1;
    cout << "Please enter second web address: "; // example: http://supercomputingblog.com
//    cin >> webAddress2;

//    getsource(webAddress1, szFileName1);
//    getsource(webAddress2, szFileName2);


    //delete html tags,script ,style,...
    string sz1;
    string sz2;
    ifstream f1;
    f1.open("result1.txt");
    getline(f1, sz1, '\0');
    f1.close();
    ifstream f2;
    f2.open("result2.txt");
    getline(f2, sz2, '\0');
    f2.close();

    regex rx("[^[:alpha:]]"),         //to remove non-alphanumeric characters
    rxx("\\s+"),                      //to remove \s (space)
    script("<script(.*?)</script>"),  //to remove <script> tags and stuff between them
    stylesheet("<style(.*?)</style>"),//to remove <style> tags and stuff between them
    htmlTags("</?(.|\n)*?>");         //to remove html tags

    sz1 = regex_replace(sz1, script, " ");
    sz1 = regex_replace(sz1, stylesheet, " ");
    sz1 = regex_replace(sz1, htmlTags, " ");
    sz1 = regex_replace(sz1, rx, " ");
    sz1 = regex_replace(sz1, rxx, " ");
    sz2 = regex_replace(sz2, script, " ");
    sz2 = regex_replace(sz2, stylesheet, " ");
    sz2 = regex_replace(sz2, htmlTags, " ");
    sz2 = regex_replace(sz2, rx, " ");
    sz2 = regex_replace(sz2, rxx, " ");

    map<string,int> stop_words;
    map<string,int> text1;
    map<string,int> text2;

    string sz3;
    string sz4;
    ifstream f3;
    f3.open("stop-word-list.txt");
    getline(f3, sz3,'\0');
    f3.close();
    replace(sz3.begin(), sz3.end(), '\r', ' ');// replace \n with space
    replace(sz3.begin(), sz3.end(), '\n', ' ');// replace \n with space
    for (size_t i = 0; i < sz1.size(); i++)
    {
        if (sz1[i] >= 'A' &&sz1[i] <= 'Z')
        {
            sz1[i] += 32;
        }
    }
    for (size_t i = 0; i < sz2.size(); i++)
    {
        if (sz2[i] >= 'A' &&sz2[i] <= 'Z')
        {
            sz2[i] += 32;
        }
    }
    split(sz3, ' ', stop_words);//put in map stopwords(stopword list exits in a file in folder of project)
    split(sz1, ' ', text1);//put in map website1's text
    split(sz2, ' ', text2);//put in map website2's text

    for (auto & i:stop_words)
    {
        if (text1.count(i.first))
            text1.erase(i.first);
        if (text2.count(i.first))
            text2.erase(i.first);
    }

    vector<int> v1;
    vector<int> v2;
    size_t size1=text1.size();
    size_t size2 = text2.size();
    if (size1 <= size2)
    {
        for (auto & i : text1)
        {
            if (text2.count(i.first))
            {
                v1.push_back(i.second);
                v2.push_back(i.second);
            }
            else
            {
                v1.push_back(i.second);
                v2.push_back(0);
            }
            text2.erase(i.first);
        }

        for(auto& i:text2)
        {
            v1.push_back(0);
            v2.push_back(i.second);
        }

    }
    else if (size1 > size2)
    {
        for (auto& i : text2)
        {
            if (text1.count(i.first))
            {
                v1.push_back(i.second);
                v2.push_back(i.second);
            }
            else
            {
                v1.push_back(i.second);
                v2.push_back(0);
            }
            text1.erase(i.first);
        }
        for(auto& i:text1)
        {
            v1.push_back(0);
            v2.push_back(i.second);
        }
    }

    //calculate similarity
    double d1d2=0, d1=0, d2=0 ,x=0,y=0;
    for (size_t i = 0; i < v1.size(); i++)
    {
        d1d2 += v1[i] * v2[i];
        x += v1[i] * v1[i];
        y += v2[i] * v2[i];
    }
    d1 = sqrt(x);
    d2 = sqrt(y);
    double similarity = 0;
    similarity =(  d1d2 / (d1*d2))*100;
    cout << endl << "***********" << similarity << "***********" << endl;
    return 0;
}
