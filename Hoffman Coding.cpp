#include <iostream>
#include <algorithm>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <queue>
#include <climits>
#include <iterator>

using namespace std;

struct TreeNode {
  int data;
  char character;
  TreeNode *left;
  TreeNode *right;
  bool parent;

  TreeNode(int d, TreeNode *l = nullptr, TreeNode *r = nullptr, bool p = false,
           char c = (char)0) {
    data = d;
    left = l;
    right = r;
    character = c;
    parent = p;
  }

  ~TreeNode() {
    delete left;
    delete right;
  }
};

typedef std::vector<pair<char, int>> freqVec;
typedef std::vector<bool> HCode;
typedef std::vector<pair<char, HCode>> HCodeVect;
typedef std::map<char, HCode> HCodeMap;

freqVec getFrequency(string str);
TreeNode *generateTree(const freqVec fv);
void GenerateCodes(const TreeNode *node, const HCode &prefix,
                   HCodeMap &outCodes);
void insert(TreeNode *&nd, int c);
void btfree(TreeNode *&root);
void postorder(TreeNode *p, int indent);

int main() {

  std::cout << "Enter input: ";
  string str;
  getline(cin, str);

  freqVec frequency = getFrequency(str);

  TreeNode *root = generateTree(frequency);
  HCodeMap map;

  // postorder(root,0);

  GenerateCodes(root, HCode(), map);

  HCodeVect HVec;
  for (auto const &x : map) {
    HVec.push_back(std::make_pair(x.first, x.second));
  }

  std::sort(HVec.begin(), HVec.end(), [](auto &left, auto &right) {
    if (left.second.size() == right.second.size())
      return left.second < right.second;
    else {

      return left.second.size() < right.second.size();
    }
  });

  for (const auto &p : HVec) {
    std::cout << p.first << " : ";
    std::copy(p.second.begin(), p.second.end(),
              std::ostream_iterator<bool>(std::cout));
    std::cout << std::endl;
  }

  btfree(root);
  delete (root);
}

void GenerateCodes(const TreeNode *node, const HCode &prefix,
                   HCodeMap &outCodes) {
  if (node->parent == true) {
    HCode leftPrefix = prefix;
    leftPrefix.push_back(false);
    GenerateCodes(node->left, leftPrefix, outCodes);

    HCode rightPrefix = prefix;
    rightPrefix.push_back(true);
    GenerateCodes(node->right, rightPrefix, outCodes);
  } else {
    outCodes[node->character] = prefix;
  }
}

struct NodeCmp {
  bool operator()(const TreeNode *lhs, const TreeNode *rhs) const {
    return lhs->data > rhs->data;
  }
};

TreeNode *generateTree(const freqVec fv) {
  std::priority_queue<TreeNode *, std::vector<TreeNode *>, NodeCmp> trees;

  for (unsigned int i = 0; i < fv.size(); ++i) {
    if (fv[i].first != 0)
      trees.push(
          new TreeNode(fv[i].second, nullptr, nullptr, false, fv[i].first));
  }
  while (trees.size() > 1) {
    TreeNode *childR = trees.top();
    trees.pop();

    TreeNode *childL = trees.top();
    trees.pop();

    TreeNode *parent =
        new TreeNode(childL->data + childR->data, childL, childR, true);
    trees.push(parent);
  }
  return trees.top();
}

freqVec getFrequency(string str) {
  std::map<char, int> freq;
  for (const char &c : str) {
    freq[c]++;
  }

  freqVec fv;
  for (auto const &x : freq) {
    fv.push_back(std::make_pair(x.first, x.second));
  }

  std::sort(fv.begin(), fv.end(),
            [](auto &left, auto &right) { return left.second < right.second; });

  return fv;
}

void insert(TreeNode *&nd, int c) {
  if (nd == 0)
    nd = new TreeNode(c);
  if (c < nd->data)
    insert(nd->left, c);
  else
    insert(nd->right, c);
  return;
}

void postorder(TreeNode *p, int indent = 0) {
  if (p != NULL) {
    if (p->right) {
      postorder(p->right, indent + 4);
    }
    if (indent) {
      std::cout << std::setw(indent) << ' ';
    }
    if (p->right)
      std::cout << " /\n" << std::setw(indent) << ' ';
    std::cout << p->data << "\n ";
    if (p->left) {
      std::cout << std::setw(indent) << ' ' << " \\\n";
      postorder(p->left, indent + 4);
    }
  }
}

void btfree(TreeNode *&root) {
  if (root != 0) {
    btfree(root->left);
    btfree(root->right);
    delete root;
    root = 0;
  }
}
