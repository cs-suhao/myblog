# 数据结构相关

## 相关习题

### LeetCode 146. LRU缓存机制

```c++
struct DNode {
    int val;
    int key;
    DNode *next, *pre;
    DNode() : key(0), val(0), next(NULL), pre(NULL) {}
    DNode(int x, int y) : key(x), val(y), next(NULL), pre(NULL) {}
};

class LRUCache {
public:
    unordered_map<int, DNode*> cache;
    DNode *head;
    DNode *tail;
    int capacity;
    int size;

    LRUCache(int _capacity) : capacity(_capacity), size(0) {
        head = new DNode();
        tail = new DNode();
        head->next = tail;
        tail->pre = head;
    }
    
    int get(int key) {
        if(!cache.count(key)){
            return -1;
        }
        DNode *node = cache[key];
        moveNode(node);
        return node->val;
    }
    
    void put(int key, int value) {
        if(!cache.count(key)){
            DNode *node = new DNode(key, value);
            cache[key] = node;
            insertNode(node);
            size++;
            if(size > capacity){
                DNode *temp = deleteFirst();
                size--;
                cache.erase(temp->key);
                delete temp;
            }
        }else{
            DNode *node = cache[key];
            node->val = value;
            moveNode(node);
        }
    }

    // insert node
    void insertNode(DNode *node){
        node->next = tail;
        node->pre = tail->pre;
        tail->pre->next = node;
        tail->pre = node;
    }

    // delete node
    void deleteNode(DNode *node){
        node->pre->next = node->next;
        node->next->pre = node->pre;
        
    }

    // delete first node
    DNode* deleteFirst(){
        DNode *node = head->next;
        node->next->pre = head;
        head->next = node->next;
        return node;
    }

    // move node
    void moveNode(DNode *node){
        deleteNode(node);
        insertNode(node);
    }

};

```


### LeetCode 895. 最大频率栈

```c++
class FreqStack {
public:
    int maxFre;
    unordered_map<int, int> key_fre;
    unordered_map<int, stack<int>> fre_stack;
    FreqStack() {
        maxFre = 0;
        key_fre.clear();
        fre_stack.clear();
    }
    
    void push(int val) {
        // fre == 0
        if(!key_fre.count(val)){
            key_fre[val] = 1;
            fre_stack[1].push(val);
        }else{
            // fre > 1
            int fre = key_fre[val];
            key_fre[val] = fre+1;
            fre_stack[fre+1].push(val);
        }
        if(key_fre[val] > maxFre) maxFre = key_fre[val];
    }
    
    int pop() {
        int val = fre_stack[maxFre].top();
        fre_stack[maxFre].pop();
        if(fre_stack[maxFre].empty()){
            maxFre--;
        }
        key_fre[val]--;
        return val;
    }
};

```


### LeetCode 460. LFU缓存

个人认为LFU缓存更加困难，关于C++概念中的`list`和`iterator`还需要加强，这一题是在参考答案的基础上写出来的。
```c++

struct Node{
    int key, val, fre;
    Node(int _key, int _val, int _fre) : key(_key), val(_val), fre(_fre) {}
};

class LFUCache {
public:
    int capacity, minFre;
    unordered_map<int, list<Node>::iterator> keyToNode;
    unordered_map<int, list<Node>> freToKey;

    LFUCache(int _capacity) : capacity(_capacity), minFre(0){
        keyToNode.clear();
        freToKey.clear();
    }
    
    int get(int key) {
        if(!capacity) return -1;
        auto iter = keyToNode.find(key);
        if(iter == keyToNode.end()) return -1;
        list<Node>::iterator node = iter->second;
        int val = node->val, fre = node->fre;
        freToKey[fre].erase(node);
        if(freToKey[fre].size() == 0){
            freToKey.erase(fre);
            if(minFre == fre) minFre++;
        }
        freToKey[fre+1].push_front(Node(key,val,fre+1));
        keyToNode[key] = freToKey[fre+1].begin();

        return val;
    }
    
    void put(int key, int value) {
        if(!capacity) return;
        auto iter = keyToNode.find(key);
        if(iter == keyToNode.end()){
            if(capacity == keyToNode.size()){
                auto leastnode = freToKey[minFre].back();
                keyToNode.erase(leastnode.key);
                freToKey[minFre].pop_back();
                if(freToKey[minFre].size() == 0){
                    freToKey.erase(minFre);
                }
            }
            freToKey[1].push_front(Node(key,value,1));
            keyToNode[key] = freToKey[1].begin();
            minFre = 1;
        }else{
            list<Node>::iterator node = iter->second;
            int fre = node->fre;
            freToKey[fre].erase(node);
            if(freToKey[fre].size() == 0){
                freToKey.erase(fre);
                if(minFre == fre) minFre++;
            }
            freToKey[fre+1].push_front(Node(key,value,fre+1));
            keyToNode[key] = freToKey[fre+1].begin();
        }


    }
};

```