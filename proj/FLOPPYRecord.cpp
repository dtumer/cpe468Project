//
//  FLOPPYRecord.cpp
//

#include "FLOPPYRecord.h"

#include <string.h>


FLOPPYRecord::FLOPPYRecord() {
    columns = new std::vector<FLOPPYRecordAttribute *>();
    
    tempNodes = new std::vector<FLOPPYValue *>();
}

FLOPPYRecord::~FLOPPYRecord() {
    for (auto itr = this->columns->begin() ; itr != this->columns->end(); itr++) {
        delete (*itr);
    }
    delete columns;
    
    
    for (auto itr = this->tempNodes->begin() ; itr != this->tempNodes->end(); itr++) {
        delete (*itr);
    }
    delete tempNodes;
}


FLOPPYValue * FLOPPYRecord::filter(FLOPPYNode *node) {
    FLOPPYValue *ret;
    
    if(node->_type == FLOPPYNodeType::ValueNode) {
        printf("ValueNode\n");
        if(node->value->type() == ValueType::TableAttributeValue) {
            printf("\t TableAttributeValue\n");
            char *tableName = node->value->tableAttribute->tableName;
            char *attribute = node->value->tableAttribute->attribute;
            
            
            
            for (unsigned i=0; i<columns->size(); i++) {
                FLOPPYRecordAttribute *col = columns->at(i);
                
                if(tableName)
                    if(strcmp(tableName, col->tableName) != 0)
                        continue;
                
                if(strcmp(attribute, col->name) != 0)
                    continue;
                
                return col->val;
            }
            
            printf("ERROR - tableAttribute not found\n");
        }
        else
        	return node->value;
    }
    else if(node->_type == FLOPPYNodeType::ConditionNode)
    {
        printf("ConditionNode\n");
        FLOPPYValue *leftRet = filter(node->node.left);
        FLOPPYValue *rightRet = filter(node->node.right);
        
        if(node->node.op == FLOPPYNodeOperator::AndOperator) {
            printf("\t AndOperator\n");
            if((leftRet->type() != ValueType::BooleanValue) || (rightRet->type() != ValueType::BooleanValue)){
                printf("ERROR - AND without bool\n");
            }
            else {
                ret = new FLOPPYValue(BooleanValue);
                tempNodes->push_back(ret);
                ret->bVal = leftRet->bVal && rightRet->bVal;
                return ret;
            }
        }
        else if(node->node.op == FLOPPYNodeOperator::GreaterThanOperator) {
            printf("\t GreaterThanOperator\n");
            
            if(leftRet->type() == rightRet->type()) {
                ret = new FLOPPYValue(BooleanValue);
                tempNodes->push_back(ret);
                
                if(leftRet->type() == ValueType::StringValue)
                    ret->bVal = (strcmp(leftRet->sVal, rightRet->sVal) > 0);
                else if(leftRet->type() == ValueType::IntValue)
                    ret->bVal = (leftRet->iVal > rightRet->iVal);
                else if(leftRet->type() == ValueType::FloatValue)
                    ret->bVal = (leftRet->fVal > rightRet->fVal);
                else if(leftRet->type() == ValueType::BooleanValue)
                    ret->bVal = (leftRet->bVal > rightRet->bVal);
                else
                    ret->bVal = 0;
                
                return ret;
            }
            else
                printf("ERROR - > with diff types\n");
        }
        else if(node->node.op == FLOPPYNodeOperator::GreaterThanEqualOperator) {
            printf("\t GreaterThanEqualOperator\n");
            
            if(leftRet->type() == rightRet->type()) {
                ret = new FLOPPYValue(BooleanValue);
                tempNodes->push_back(ret);
                
                if(leftRet->type() == ValueType::StringValue)
                    ret->bVal = (strcmp(leftRet->sVal, rightRet->sVal) >= 0);
                else if(leftRet->type() == ValueType::IntValue)
                    ret->bVal = (leftRet->iVal >= rightRet->iVal);
                else if(leftRet->type() == ValueType::FloatValue)
                    ret->bVal = (leftRet->fVal >= rightRet->fVal);
                else if(leftRet->type() == ValueType::BooleanValue)
                    ret->bVal = (leftRet->bVal >= rightRet->bVal);
                else
                    ret->bVal = 0;
                
                return ret;
            }
            else
                printf("ERROR - >= with diff types\n");
        }
        else if(node->node.op == FLOPPYNodeOperator::LessThanOperator) {
            printf("\t LessThanOperator\n");
            
            if(leftRet->type() == rightRet->type()) {
                ret = new FLOPPYValue(BooleanValue);
                tempNodes->push_back(ret);
                
                if(leftRet->type() == ValueType::StringValue)
                    ret->bVal = (strcmp(leftRet->sVal, rightRet->sVal) < 0);
                else if(leftRet->type() == ValueType::IntValue)
                    ret->bVal = (leftRet->iVal < rightRet->iVal);
                else if(leftRet->type() == ValueType::FloatValue)
                    ret->bVal = (leftRet->fVal < rightRet->fVal);
                else if(leftRet->type() == ValueType::BooleanValue)
                    ret->bVal = (leftRet->bVal < rightRet->bVal);
                else
                    ret->bVal = 0;
                
                return ret;
            }
            else
                printf("ERROR - < with diff types\n");
        }
        else if(node->node.op == FLOPPYNodeOperator::LessThanEqualOperator) {
            printf("\t LessThanEqualOperator\n");
            
            if(leftRet->type() == rightRet->type()) {
                ret = new FLOPPYValue(BooleanValue);
                tempNodes->push_back(ret);
                
                if(leftRet->type() == ValueType::StringValue)
                    ret->bVal = (strcmp(leftRet->sVal, rightRet->sVal) <= 0);
                else if(leftRet->type() == ValueType::IntValue)
                    ret->bVal = (leftRet->iVal <= rightRet->iVal);
                else if(leftRet->type() == ValueType::FloatValue)
                    ret->bVal = (leftRet->fVal <= rightRet->fVal);
                else if(leftRet->type() == ValueType::BooleanValue)
                    ret->bVal = (leftRet->bVal <= rightRet->bVal);
                else
                    ret->bVal = 0;
                
                return ret;
            }
            else
                printf("ERROR - <= with diff types\n");
        }
        else if(node->node.op == FLOPPYNodeOperator::EqualOperator) {
            printf("\t EqualOperator\n");
            
            if(leftRet->type() == rightRet->type()) {
                ret = new FLOPPYValue(BooleanValue);
                tempNodes->push_back(ret);
                
                if(leftRet->type() == ValueType::StringValue)
                    ret->bVal = (strcmp(leftRet->sVal, rightRet->sVal) == 0);
                else if(leftRet->type() == ValueType::IntValue)
                    ret->bVal = (leftRet->iVal == rightRet->iVal);
                else if(leftRet->type() == ValueType::FloatValue)
                    ret->bVal = (leftRet->fVal == rightRet->fVal);
                else if(leftRet->type() == ValueType::BooleanValue)
                    ret->bVal = (leftRet->bVal == rightRet->bVal);
                else
                    ret->bVal = 0;
                
                return ret;
            }
            else
                printf("ERROR - == with diff types\n");
        }
        else if(node->node.op == FLOPPYNodeOperator::NotEqualOperator) {
            printf("\t NotEqualOperator\n");
            
            if(leftRet->type() == rightRet->type()) {
                ret = new FLOPPYValue(BooleanValue);
                tempNodes->push_back(ret);
                
                if(leftRet->type() == ValueType::StringValue)
                    ret->bVal = (strcmp(leftRet->sVal, rightRet->sVal) != 0);
                else if(leftRet->type() == ValueType::IntValue)
                    ret->bVal = (leftRet->iVal != rightRet->iVal);
                else if(leftRet->type() == ValueType::FloatValue)
                    ret->bVal = (leftRet->fVal != rightRet->fVal);
                else if(leftRet->type() == ValueType::BooleanValue)
                    ret->bVal = (leftRet->bVal != rightRet->bVal);
                else
                    ret->bVal = 0;
                
                return ret;
            }
            else
                printf("ERROR - != with diff types\n");
        }
        /*
        else if(node->node.op == FLOPPYNodeOperator::NotOperator) {
            printf("NOT ");
            printFLOPPYNode(tabLvl+1, node->node.left);
            printf("\n%s", std::string(tabLvl, '\t').c_str());
        }
        else if(node->node.op == FLOPPYNodeOperator::GreaterThanOperator) {
            printFLOPPYNode(tabLvl+1, node->node.left);
            printf(" > ");
            printFLOPPYNode(tabLvl+1, node->node.right);
        }
        else if(node->node.op == FLOPPYNodeOperator::GreaterThanEqualOperator) {
            printFLOPPYNode(tabLvl+1, node->node.left);
            printf(" >= ");
            printFLOPPYNode(tabLvl+1, node->node.right);
        }
        else if(node->node.op == FLOPPYNodeOperator::LessThanOperator) {
            printFLOPPYNode(tabLvl+1, node->node.left);
            printf(" < ");
            printFLOPPYNode(tabLvl+1, node->node.right);
        }
        else if(node->node.op == FLOPPYNodeOperator::LessThanEqualOperator) {
            printFLOPPYNode(tabLvl+1, node->node.left);
            printf(" <= ");
            printFLOPPYNode(tabLvl+1, node->node.right);
        }
        else if(node->node.op == FLOPPYNodeOperator::EqualOperator) {
            printFLOPPYNode(tabLvl+1, node->node.left);
            printf(" = ");
            printFLOPPYNode(tabLvl+1, node->node.right);
        }
        else if(node->node.op == FLOPPYNodeOperator::NotEqualOperator) {
            printFLOPPYNode(tabLvl+1, node->node.left);
            printf(" != ");
            printFLOPPYNode(tabLvl+1, node->node.right);
        }
        else if(node->node.op == FLOPPYNodeOperator::ParenthesisOperator) {
            printf("Parenthesis ");
            printFLOPPYNode(tabLvl+1, node->node.left);
            printf("\n%s", std::string(tabLvl, '\t').c_str());
        }
         */
        
    }
    
    ret = new FLOPPYValue(NullValue);
    return ret;
}