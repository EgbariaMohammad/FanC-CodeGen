//
// Created by Baraa Egbaria on 10/08/2024.
// Copyright © 2024 Baraa Egbaria All rights reserved.
//
// ScopeManager.cpp

#include "ScopeManager.hpp"
#include "Symbol.hpp"
#include "hw3_output.hpp"
#include <vector>
#include <string>
#include <memory>

using namespace std;

ScopeManager::ScopeManager() : offsets(stack<int>()), scopes(vector<pair<unique_ptr<SymTable>, ScopeType>>())
{
    offsets.push(0);
    pushScope(ScopeType::GLOBAL);
}

void ScopeManager::pushScope(ScopeType type)
{
    offsets.push(0);
    scopes.push_back({std::unique_ptr<SymTable>(new SymTable), type}); // replace with make unique
}

void ScopeManager::popScope()
{
    if (scopes.size() == 0)
        throw EmptyManager();
    offsets.pop();
    scopes.pop_back();
}

void ScopeManager::insertSymbol(const shared_ptr<Symbol>& e)
{
    scopes.back().first->AddSymbol(e);
    offsets.top()++;
}

int ScopeManager::findSymbol(const string& name) const
{
    for (int i = scopes.size() - 1; i >= 0; i--)
    {
        int index = scopes[i].first->findSymbol(name);
        if (index != -1)
            return index;
    }
    return -1;
}

int ScopeManager::findSymbolInCurrentScope(const string& name) const
{
    int i = scopes.size() - 1;
    return scopes[i].first->findSymbol(name);
}

int ScopeManager::getOffset() const
{
    return offsets.top();
}

void ScopeManager::printScope() const
{
    int scopeNum = 0;
    for (const auto& scope : scopes)
    {
        std::cout << "Scope number " << scopeNum++ << ":\n";
        scope.first->printSymTable();
    }
}

ScopeType ScopeManager::getLatestScopeType() const {
    return scopes.back().second;
}

shared_ptr<Symbol> ScopeManager::getSymbol(const string& name) const
{
    for (int i = scopes.size() - 1; i >= 0; i--)
    {
        shared_ptr<Symbol> symbol = scopes[i].first->getSymbol(name);
        if (symbol != nullptr)
            return symbol;
    }
    return nullptr;
}

bool ScopeManager::searchIfScopeOpen(ScopeType type) const {
    for (int i = scopes.size() - 1; i >= 0; i--)
    {
        if (scopes[i].second == type)
            return true;
    }
    return false;
}