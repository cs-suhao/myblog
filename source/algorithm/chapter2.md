# 单链表
## 合并两个有序链表

这一题直接使用双指针，因为两个链表是有序的，因此只需要挨个比较即可。
```cpp
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
        ListNode* head = new ListNode(-1);
        ListNode* p = head;
        
        while(list1 != NULL && list2 != NULL){
            if(list1->val >= list2->val){
                p->next = list2;
                list2 = list2->next;
            }else{
                p->next = list1;
                list1 = list1->next;
            }
            p = p->next;
        }
        if(list1 == NULL){
            p->next = list2;
        }
        if(list2 == NULL){
            p->next = list1;
        }
        
        return head->next;
    }
};
```
这里需要查询并明确的地方是：如何初始化结构体，如果涉及到结构体指针，如何进行初始化？

