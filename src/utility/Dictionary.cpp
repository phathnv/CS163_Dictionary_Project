#include "Dictionary.h"

Word::Word(const std::string& s)
{
    data = s;
}

Definition::Definition(const std::string& s)
{
    data = s;
}

Dictionary::Dictionary(const std::string& _dir, const std::string &chars)
{   
    dir = _dir;
    trie = new Trie<Word*>(chars, nullptr);
    LoadData();
    LoadHistory();
}

Dictionary::~Dictionary()
{   
    saveData();
    saveHistory();
    for (auto def : allDefs)
    {
        delete def;
    }
    allDefs.clear();
    history.clear();
    delete trie;
}


void Dictionary::LoadHistory()
{   
    std::string filePath = dir + "/history.txt";
    std::ifstream fin(filePath);
    if (!fin.is_open())
    {
        std::cerr << "Could not load file " << filePath << std::endl;
        exit(0);
    }
    std::string line;
    while (getline(fin, line))
    {
        Word* temp;
        trie->find(line, temp);
        history.push_back(temp);
    }
    fin.close();
}

void Dictionary::updateHistory(Word* word)
{
    for (int i = 0; i < (int)history.size(); i++)
        if (history[i] == word)
        {
            history.erase(history.begin() + i);
        }
    history.insert(history.begin(), word);
    if (history.size() > HISTORY_LIMIT)
        history.pop_back();
    
}

void Dictionary::LoadData()
{
    std::string filePath = dir + "/data.txt";
    std::ifstream fin(filePath);
    if (!fin.is_open())
    {
        std::cerr << "Could not load file " << filePath << std::endl;
        exit(0);
    }

    std::string line;
    int lineNum = 0;
    while (getline(fin, line))
    {
        ++lineNum;
        auto tmp = Split(line, '\t');
        
        if (tmp.size() != 2) {
            std::cerr << "in file: " << filePath << ":" << std::endl;
            std::cerr << "skip line " << lineNum << ": " << line << std::endl;
        } else {
            Word* word;
            if (trie->find(tmp[0], word) != success) {
                word = new Word(tmp[0]);
                if (trie->insert(tmp[0], word) != success) {
                    std::cerr << "in file: " << filePath << ":" << std::endl;
                    std::cerr << "could not insert word " << tmp[0] << std::endl;
                    continue;
                }
            }
            Definition* def = new Definition(tmp[1]);
            word->defs.push_back(def);
            def->word = word;
            allDefs.push_back(def);
        }
    }

    fin.close();
}

std::vector<Word*> Dictionary::SearchWord(const std::string& key)
{
    if (key != "") return trie->search(key);
    return getSearchHistory();
}

std::vector<Word*> Dictionary::SearchDef(const std::string& key)
{
    std::vector<Word*> results;
    for (auto def : allDefs)
    {
        if (IsPrefix(key, def->data) && results.size() < SEARCH_RESULTS_LIMIT)
        {
            results.push_back(def->word);
        }
    }
    return results;
}

std::vector<std::string> Dictionary::getFullDefinition(const std::string& word) {
    Word* ptr;
    if (trie->find(word, ptr) != success) {
        std::cerr << "Word " << word << " not exist.";
        return std::vector<std::string>();
    }
    updateHistory(ptr);
    
    std::vector<std::string> defs;
    for (auto def : ptr->defs) {
        defs.push_back(def->data);
    }
    return defs;
}

std::vector<Word*> Dictionary::getSearchHistory()
/* Returns the latest searched words, up to 20 records
*/
{
    std::vector<Word*> result = history;
    return result;
}

void Dictionary::saveData() 
{
    // implement later
}

void Dictionary::saveHistory() 
{
    std::ofstream fout(dir + "/history.txt");
    if (!fout.is_open()) {
        std::cerr << "could not save file" + dir + "/history.txt" << std::endl;
    } else {
        for (int i = 0; i < (int)history.size(); i++)
            fout << history[i]->data << "\n";
    }
    fout.close();
}

std::vector<std::string> Split(const std::string& s, char delim)
{
    std::string tmp;
    std::vector<std::string> res;
    for (char ch : s)
    {
        if (ch == delim)
        {
            res.push_back(tmp);
            tmp.clear();
        }
        else
        {
            tmp.push_back(ch);
        }
    }
    res.push_back(tmp);
    return res;
}

bool IsPrefix(const std::string& p, const std::string& s)
{
    if (p.size() > s.size())
    {
        return false;
    }
    return (p == s.substr(0, p.size()));
}