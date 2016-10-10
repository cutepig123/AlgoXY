#include <vector>
#include <deque>
#include <string>
#include <map>
#include <algorithm>
#include <assert.h>

using namespace std;

 //* Definition for singly-linked list.
  struct ListNode {
      int val;
      ListNode *next;
      ListNode(int x) : val(x), next(NULL) {}
  };

class Solution {
public:
    int length(ListNode* h)
    {
        int n=0;
        while (h) {
            h=h->next;n++;
        }
        return n;
    }


    ListNode* removeNthFromEnd(ListNode* head, int n) {
        //int n=length(head);
        int t=0;
        if(n==0)
            return head->next;

        ListNode *h=head;
        while (h && t+1<n) {
            h=h->next;t++;
        }

        if(h->next)
            h->next = h->next->next;

        return head;
    }

    ListNode* create(vector<int> const &v)
    {
        ListNode *h=0, *t=0;
        for(auto i:v)
        {
            if(!h)
                h=t=new ListNode(i);
            else
            {
                t->next=new ListNode(i);
                t=t->next;
            }
        }
        return h;
    }

    vector<int> toVec(ListNode *h)
    {
        vector<int> v;
        while (h) {
            v.push_back(h->val);
            h=h->next;
        }
        return move(v);
    }
};

void testq19()
{
Solution s;
ListNode *h= s.create(vector<int>{1,2,3,4,5});
ListNode *h2= s.removeNthFromEnd(h,2);
vector<int> v2= s.toVec(h2);
}
