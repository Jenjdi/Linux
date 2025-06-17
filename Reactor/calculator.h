#pragma once
#include"packageparse.h"
class Calculator
{
    public:
    std::shared_ptr<Response> calculator(std::shared_ptr<Request> req)
    {
        auto resp=Factory::BuildResponse();
        switch(req->oper())
        {
            case '+':
            resp->_result=req->X()+req->Y();
            break;
            case '-':
            resp->_result=req->X()-req->Y();
            break;
            case '/':
            if(req->Y()==0)
            {
                resp->_code=1;
                resp->_desc="divide zero";
                
            }
            else
                resp->_result=req->X()/req->Y();
            break;
            break;
            case '*':
            resp->_result=req->X()*req->Y();
            break;
            case '%':
            if(req->Y()==0)
            {
                resp->_code=2;
                resp->_desc="mod zero";
            }
            else
                resp->_result=req->X()%req->Y();
            break;
            case '^':
            resp->_result=req->X()^req->Y();
            break;
            case '&':
            resp->_result=req->X()&req->Y();
            break;
        }
        return resp;
    }
};