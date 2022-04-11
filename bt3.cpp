#include<bits/stdc++.h>
using namespace std;

class Node
{
public:
    vector<int> values;
    int size;
    vector<Node*> children;
    bool isDataNode;
    Node* parent;

    Node(int s, bool dataNode)
    {
        size= s;
        isDataNode= dataNode;
        parent=NULL;
    }
};

bool comp(Node* c1, Node* c2)               //compare
{
    return c1->values[0]< c2->values[0];
}

class BPTree
{
public:
    int d;
    int t;
    int indexNodesCount;
    int dataNodesCount;

    Node *root;

    BPTree(int dsize, int  tsize)
    {
        d= dsize;
        t= tsize;
        root= NULL;

        indexNodesCount=0;
        dataNodesCount=0;
    }

    void insert(int ele)
    {
        if(root==NULL)
        {
            root= new Node(d, true);
            vector<int> &v= root->values;
            v.push_back(ele);
            sort(v.begin(), v.end());
            dataNodesCount++;
        }
        else
        {
            //find the right data node
            Node *n= root;
            Node *prev=NULL;
            int childIndex=-1;
            while(!n->isDataNode)
            {
                //check to find the correct data node
                //else go to last child of the index node and insert there
                vector<Node*> &childNodes= n->children;
                int k= childNodes.size();
                Node *c= childNodes[k-1];
                childIndex= k-1;
                for(int j=1; j<k; j++)
                {
                    if(ele < childNodes[j]->values[0])
                    {
                        c= childNodes[j-1];
                        childIndex= j-1;
                        break;
                    }
                }
                prev=n;
                n=c;
            }

            //insert and sort
            vector<int> &v= n->values;
            v.push_back(ele);
            sort(v.begin(), v.end());

            //after inserting check overflow then split
            if(n->values.size() <= 2*(n->size))
            {
                //no overflow, then if prev just update it
                //if splitting happens then numbers added
                //if no splitting

                /*
                if splitting happened - one more index node would have been created
                if not - then the index value can change

                */
            }
            else
            {
                //overflow split
                //create 2 new data nodes
                Node* n1= new Node(d, true);
                Node* n2= new Node(d, true);

                //add elements from 0 to d
                //d to end
                vector<int> &eles= n->values;
                int i=0;
                for(;i<d && i<eles.size();i++)
                {
                    n1->values.push_back(eles[i]);
                }

                while(i<eles.size())
                {
                    n2->values.push_back(eles[i]);
                    i++;
                }

                dataNodesCount++;  //2 added and one will be deleted - n
                delete(n);

                //create a new index node or add to prev
                if(prev==NULL)
                {
                     Node* index= new Node(t, false);
                     index->values.push_back(n2->values[0]);

                     index->children.push_back(n1);
                     index->children.push_back(n2);

                     indexNodesCount++;

                     //need to update root too
                     root= index;
                }
                else
                {
                    //or modify the current index node
                    vector<Node*> &indexChildren= prev->children;
                    indexChildren.erase(indexChildren.begin() + childIndex);
                    indexChildren.push_back(n1);
                    indexChildren.push_back(n2);
                    sort(indexChildren.begin(), indexChildren.end(), comp);

                    prev->values.clear();
                    int l= indexChildren.size();
                    for(int m=1; m<l; m++)
                    {
                        prev->values.push_back(indexChildren[m]->values[0]);
                    }

                    /*cout<<"\nprinting prev values: ";
                    for(auto v: prev->values)
                    {
                        cout<<v<<" ";
                    }*/

                    //now if index node's size exceeds 2t then you will need to split index node
                    //can create 2 ndes index nodes
                    //update root too
                    if(prev->values.size()> (2*t+1))
                    {
                        //split the index node
                        //create 2 new index nodes
                        Node* i1= new Node(t, false);
                        Node* i2= new Node(t, false);

                        //add elements from 0 to t
                        //t to end
                        vector<int> &eles= prev->values;
                        int i=0;
                        for(;i<t;i++)
                        {
                            i1->values.push_back(eles[i]);
                        }

                        int a = eles[t];
                        i=t+1;
                        while(i<eles.size())
                        {
                            i2->values.push_back(eles[i]);
                            i++;
                        }

                        vector<Node*> &idxChilds= prev->children;
                        i=0;
                        for(;i<=t;i++)
                        {
                            i1->children.push_back(idxChilds[i]);
                        }

                        while(i<idxChilds.size())
                        {
                            i2->children.push_back(idxChilds[i]);
                            i++;
                        }

                        indexNodesCount+= 2;  //2 added and one will be deleted - n

                        //create one index node to point to those 2 nodes
                        Node* newIndex= new Node(t, false);
                        newIndex->values.push_back(a);

                        newIndex->children.push_back(i1);
                        newIndex->children.push_back(i2);

                        if(root==prev)
                        {
                            root= newIndex;
                        }
                        delete(prev);
                    }
                }
            }
        }
    }

    void displayHelper(Node *n)
    {
        if(!n)
            return;
        cout<<"\nPrinting node";
        cout<<"\nIs Data Node: "<<n->isDataNode;
        cout<<"\nNode values: ";
        for(auto it: n->values)
        {
            cout<<it<<" ";
        }

        cout<<"\nNode children: "<<n->children.size()<<endl;

        for(auto child: n->children)
        {
            displayHelper(child);
        }
    }

    void display()
    {
        cout<<indexNodesCount<<" "<<dataNodesCount<<endl;
        for(auto it: root->values)
        {
            cout<<it<<" ";
        }
        cout<<endl;

        displayHelper(root);
    }

};

int main()
{
    int d,t;
    cin>>d;
    cin>>t;

    int choice;
    cin>>choice;

    BPTree bpTree(d,t);

    while(choice!=3)
    {
        if(choice == 1)
        {
            int ele;
            cin>>ele;
            //insert ele to b tree
            bpTree.insert(ele);
        }
        else if(choice ==2)
        {
            bpTree.display();
        }

        cin>>choice;
    }
}
