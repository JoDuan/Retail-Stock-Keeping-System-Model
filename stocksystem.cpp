// File:        stocksystem.h
// Author:      Meihui Yan & Jiameng Duan
// Date:        2016-03-14
// Description: Declaration and partial implementation of a StockSystem class for use with CMPT 225 assignment #4

#include <math.h>
#include <sstream>

#include "stocksystem.h"

// default constructor;
// begin with a balance of $100,000.00
StockSystem::StockSystem()
{
	balance = 100000.00;
}

// returns the balance member
double StockSystem::GetBalance()
{
	return balance;
}

// Add a new SKU to the system. Do not allow insertion of duplicate sku
bool StockSystem::StockNewItem(StockItem item)
{
	return(records.Insert(item));
}

// Locate the item with key itemsku and update its description field.
// Return false if itemsku is not found.
bool StockSystem::EditStockItemDescription(int itemsku, string desc)
{
	StockItem item = StockItem(itemsku, desc, 0);
	StockItem* temp = records.Retrieve(item);
	if (temp != nullptr)
	{
		temp->SetDescription(desc);
		return true;
	}
	else
		return false;
}

// Locate the item with key itemsku and update its description field.
// Return false if itemsku is not found or retailprice is negative.
bool StockSystem::EditStockItemPrice(int itemsku, double retailprice)
{
	StockItem item = StockItem(itemsku, "", 0);
	StockItem* temp = records.Retrieve(item);
	if (temp != nullptr && retailprice >= 0)
	{
		temp->SetPrice(retailprice);
		return true;
	}
	else
		return false;
}

// Purchase quantity of item at unitprice each, to reach a maximum (post-purchase) on-hand stock quantity of 1000.
// Return false if balance is not sufficient to make the purchase,
//   or if SKU does not exist, or if quantity or unitprice are negative.
// Otherwise, return true and increase the item's on-hand stock by quantity,
//   and reduce balance by quantity*unitprice.
bool StockSystem::Restock(int itemsku, int quantity, double unitprice)
{
	bool balEnough, existence;
	if (balance > quantity*unitprice)
		balEnough = true;
	else
		balEnough = false;

	StockItem item = StockItem(itemsku, "", 0);
	StockItem* temp = records.Retrieve(item);
	if (temp == nullptr)
		existence = false;
	else
		existence = true;

	if (!balEnough || !existence || (quantity < 0) || (unitprice < 0))
		return false;
	else
	{
		temp->SetStock(temp->GetStock() + quantity);
		balance = balance - quantity*unitprice;
		return true;
	}
}

// Sell an item to a customer, if quantity of stock is available and SKU exists.
// Reduce stock by quantity, increase balance by quantity*price, and return true if stock available.
// If partial stock (less than quantity) available, sell the available stock and return true.
// If no stock, sku does not exist, or quantity is negative, return false.
bool StockSystem::Sell(int itemsku, int quantity)
{
	bool existence;
	StockItem item = StockItem(itemsku, "", 0);
	StockItem* temp = records.Retrieve(item);
	if (temp == nullptr)
		existence = false;
	else
		existence = true;
	
	if (existence && temp->GetStock() > quantity)
	{
		temp->SetStock(temp->GetStock() - quantity);
		balance = balance + quantity*(temp->GetPrice());
		return true;
	}
	else if ((0 <= quantity < temp->GetStock()) && existence)
	{
		balance = balance + (temp->GetStock())*(temp->GetPrice());
		temp->SetStock(0);
		return true;
	}
	else
		return false;
}