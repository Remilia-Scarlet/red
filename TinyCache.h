#include <list>
#include <algorithm>
#include <functional>
template<typename Key,typename Val>
class TinyCache
{
public:
    TinyCache(int capacity)
        :_capacity(capacity)
    {
        _data.resize(capacity,nullptr);
    }
    ~TinyCache()
    {
        clearCache();
    }
    Val getCache(const Key& key,const std::function<Val()>& searchFunction)
    {
        auto it = find_if(_data.begin(),_data.end(),[&key](KeyValPair*& val){
            if(val && val->key == key)
                return true;
            return false;
        });
        
        if(it != _data.end())
            updateData(it);
        else
            addCache(key,searchFunction());
        
        return _data.front()->val;
    }
    void addCacheData(const Key& key,const Val& val)
    {
        auto it = find_if(_data.begin(),_data.end(),[&key](KeyValPair*& val){
            if(val && val->key == key)
                return true;
            return false;
        });
        if(it != _data.end())
            updateData(it);
        else
            addCache(key,val);
    }
    void clearCache()
    {
        for(auto it = _data.begin();it != _data.end();++it)
        {
            if(*it != nullptr)
            {
                delete *it;
                *it = nullptr;
            }
        }
    }
private:
    struct KeyValPair
    {
        KeyValPair(Key key,Val val):key(key),val(val){}
        Key key;
        Val val;
    };
    std::list<KeyValPair*> _data;
    int _capacity;
    
    TinyCache(const TinyCache& other);
    TinyCache& operator=(const TinyCache& other);
    void addCache(const Key& key,const Val& val)
    {
        KeyValPair* old = _data.back();
        if(old == nullptr)
            old = new KeyValPair(key,val);
        else
        {
            old->key = key;
            old->val = val;
        }
        _data.pop_back();
        _data.push_front(old);
    }
    void updateData(typename std::list<KeyValPair*>::iterator& it)
    {
        KeyValPair* da = *it;
        if(it != _data.begin())
        {
            _data.erase(it);
            _data.push_front(da);
        }
    }
};
