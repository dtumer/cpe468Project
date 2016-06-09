//
//  FLOPPYRecord.cpp
//

#include "FLOPPYRecord.h"

#include <string.h>
#include <math.h>


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
            
//             printf("ATTRIBUTE NAME: ;%s;\n", attribute);
            
            for (unsigned i=0; i<columns->size(); i++) {
                FLOPPYRecordAttribute *col = columns->at(i);
                
                if (tableName) {
                	if (strcmp(tableName, col->tableName) != 0) {
                		continue;
                	}
                }
                
                // printf("COL NAME: ;%s;\n", col->name);
//                 printf("ATTRIBUTE NAME: ;%s;\n", attribute);
//                 printf("COMPARE: %d\n", strcmp(attribute, col->name));
                if (0 != strcmp(attribute, col->name)) {
                	continue;
                }
                
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
        FLOPPYValue *rightRet;
        
        if (node->node.op != FLOPPYNodeOperator::ParenthesisOperator && node->node.op != FLOPPYNodeOperator::NotOperator) {
        	rightRet = filter(node->node.right);
        }
        
        if (node->node.op == FLOPPYNodeOperator::NotOperator) {
        	printf("\t NotOperator\n");
        	
        	return leftRet;
        }
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
                
                if(leftRet->type() == ValueType::StringValue) {
                	ret->bVal = (strcmp(leftRet->sVal, rightRet->sVal) == 0);
                }
//                     ret->bVal = (strcmp(leftRet->sVal, rightRet->sVal) == 0);
                else if(leftRet->type() == ValueType::IntValue)
                    ret->bVal = (leftRet->iVal == rightRet->iVal);
                else if(leftRet->type() == ValueType::FloatValue)
                    ret->bVal = (leftRet->fVal == rightRet->fVal);
                else if(leftRet->type() == ValueType::BooleanValue)
                    ret->bVal = (leftRet->bVal == rightRet->bVal);
                else
                    ret->bVal = 0;
                
                printf("HERE\n");
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
        else if(node->node.op == FLOPPYNodeOperator::ParenthesisOperator) {
            printf("\t ParenthesisOperator\n");
            
            return leftRet;
        }
        else {
        	printf("ERROR - operation in condition not found\n");
        }
    }
    else if (node->_type == FLOPPYNodeType::ExpressionNode) {
    	FLOPPYValue *leftRet = filter(node->node.left);
        FLOPPYValue *rightRet;
        
        if (node->node.op != FLOPPYNodeOperator::ParenthesisOperator) {
        	rightRet = filter(node->node.right);
        }
        
    	if(node->node.op == FLOPPYNodeOperator::PlusOperator) {
            printf("\t PlusOperator\n");

            if(leftRet->type() == rightRet->type()) {
                if(leftRet->type() == ValueType::IntValue) {
                	ret = new FLOPPYValue(IntValue);
                	tempNodes->push_back(ret);
                	
                	ret->iVal = leftRet->iVal + rightRet->iVal;
                	
                	return ret;
                }
                else if(leftRet->type() == ValueType::FloatValue) {
                	ret = new FLOPPYValue(FloatValue);
                	tempNodes->push_back(ret);
                	
                	ret->fVal = leftRet->fVal + rightRet->fVal;
                	
                	return ret;
                }
            }
            else {
            	printf("ERROR - '+' with diff types\n");
            	
            	ret = new FLOPPYValue(NullValue);
                tempNodes->push_back(ret);
                
                return ret;
            }
        }
        else if(node->node.op == FLOPPYNodeOperator::MinusOperator) {
            printf("\t MinusOperator\n");

            if(leftRet->type() == rightRet->type()) {
                if(leftRet->type() == ValueType::IntValue) {
                	ret = new FLOPPYValue(IntValue);
                	tempNodes->push_back(ret);
                	
                	ret->iVal = leftRet->iVal + rightRet->iVal;
                	
                	return ret;
                }
                else if(leftRet->type() == ValueType::FloatValue) {
                	ret = new FLOPPYValue(FloatValue);
                	tempNodes->push_back(ret);
                	
                	ret->fVal = leftRet->fVal + rightRet->fVal;
                	
                	return ret;
                }
            }
            else {
            	printf("ERROR - '-' with diff types\n");
            	
            	ret = new FLOPPYValue(NullValue);
                tempNodes->push_back(ret);
                
                return ret;
            }
        }
        else if(node->node.op == FLOPPYNodeOperator::TimesOperator) {
            printf("\t TimesOperator\n");

            if(leftRet->type() == rightRet->type()) {
                if(leftRet->type() == ValueType::IntValue) {
                	ret = new FLOPPYValue(IntValue);
                	tempNodes->push_back(ret);
                	
                	ret->iVal = leftRet->iVal * rightRet->iVal;
                	
                	return ret;
                }
                else if(leftRet->type() == ValueType::FloatValue) {
                	ret = new FLOPPYValue(FloatValue);
                	tempNodes->push_back(ret);
                	
                	ret->fVal = leftRet->fVal * rightRet->fVal;
                	
                	return ret;
                }
            }
            else {
            	printf("ERROR - '*' with diff types\n");
            	
            	ret = new FLOPPYValue(NullValue);
                tempNodes->push_back(ret);
                
                return ret;
            }
        }
        else if(node->node.op == FLOPPYNodeOperator::DivideOperator) {
            printf("\t DivideOperator\n");

            if(leftRet->type() == rightRet->type()) {
                if(leftRet->type() == ValueType::IntValue) {
                	ret = new FLOPPYValue(IntValue);
                	tempNodes->push_back(ret);
                	
                	ret->iVal = leftRet->iVal / rightRet->iVal;
                	
                	return ret;
                }
                else if(leftRet->type() == ValueType::FloatValue) {
                	ret = new FLOPPYValue(FloatValue);
                	tempNodes->push_back(ret);
                	
                	ret->fVal = leftRet->fVal / rightRet->fVal;
                	
                	return ret;
                }
            }
            else {
            	printf("ERROR - '/' with diff types\n");
            	
            	ret = new FLOPPYValue(NullValue);
                tempNodes->push_back(ret);
                
                return ret;
            }
        }
        else if(node->node.op == FLOPPYNodeOperator::ModOperator) {
            printf("\t ModOperator\n");

            if(leftRet->type() == rightRet->type()) {
                if(leftRet->type() == ValueType::IntValue) {
                	ret = new FLOPPYValue(IntValue);
                	tempNodes->push_back(ret);
                	
                	ret->iVal = leftRet->iVal % rightRet->iVal;
                	
                	return ret;
                }
                else if(leftRet->type() == ValueType::FloatValue) {
                	ret = new FLOPPYValue(FloatValue);
                	tempNodes->push_back(ret);
                	
                	ret->fVal = fmod(leftRet->fVal, rightRet->fVal);
                	
                	return ret;
                }
            }
            else {
            	printf("ERROR - '%%' with diff types\n");
            	
            	ret = new FLOPPYValue(NullValue);
                tempNodes->push_back(ret);
                
                return ret;
            }
        }
        else if(node->node.op == FLOPPYNodeOperator::ParenthesisOperator) {
            printf("\t ParenthesisOperator\n");
            
            return leftRet;
        }
        else {
        	printf("ERROR - operation in expression not found\n");
        }
    }
    else if (node->_type == FLOPPYNodeType::AggregateNode) {
    	printf("AGGREGATE\n");
    }
    
    ret = new FLOPPYValue(NullValue);
    return ret;
}