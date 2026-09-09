/**
 * @file exer_21_11.cpp
 * @author KaKaRot
 * @brief
 * @version 0.1
 * @date 2026-09-08
 *
 * @copyright Copyright (c) 2026
 */

#include "Order.h"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Browser.H>
#include <FL/fl_ask.H>
#include <cstdlib>
#include <string>
#include <vector>

#ifndef DATA_DIR
#error "DATA_DIR must be defined by CMake. Use cmake to build this project."
#endif

const std::string DATA = DATA_DIR;

namespace {

struct OrderEntryState {
    Fl_Input*     customer;
    Fl_Input*     address;
    Fl_Int_Input* year;
    Fl_Int_Input* month;
    Fl_Int_Input* day;
    Fl_Input*     filename;
    Fl_Input*     product;
    Fl_Input*     price;
    Fl_Int_Input* count;
    Fl_Browser*   purchase_list;
    std::vector<Purchase> pending;
};

void add_purchase_cb(Fl_Widget*, void* v)
{
    auto* s = static_cast<OrderEntryState*>(v);
    std::string name = s->product->value();
    double price = 0.0;
    try {
        price = std::stod(s->price->value());
    } catch (...) {
        fl_alert("Invalid price");
        return;
    }
    int cnt = std::atoi(s->count->value());
    if (name.empty() || cnt <= 0) {
        fl_alert("Invalid product/count");
        return;
    }

    s->pending.emplace_back(name, price, cnt);
    std::string line = name + "  x" + std::to_string(cnt) + "  @ " + s->price->value();
    s->purchase_list->add(line.c_str());

    s->product->value("");
    s->price->value("");
    s->count->value("");
}

void save_order_cb(Fl_Widget*, void* v)
{
    auto* s = static_cast<OrderEntryState*>(v);
    if (s->pending.empty()) {
        fl_alert("No purchases added");
        return;
    }
    std::string fname = s->filename->value();
    if (fname.empty()) {
        fl_alert("Filename required");
        return;
    }

    date::year_month_day d = date::year{std::atoi(s->year->value())}
                             / std::atoi(s->month->value())
                             / std::atoi(s->day->value());
    if (!d.ok()) {
        fl_alert("Invalid date");
        return;
    }

    Order order(s->customer->value(), s->address->value(), d, s->pending);
    append_order(DATA + fname, order);

    s->pending.clear();
    s->purchase_list->clear();
    s->customer->value("");
    s->address->value("");
    s->filename->value("");
    fl_alert("Order saved.");
}

}  // namespace

int main()
{
    OrderEntryState state{};
    Fl_Window win(420, 420, "Order Entry");

    state.customer = new Fl_Input(120, 10, 280, 25, "Customer:");
    state.address  = new Fl_Input(120, 40, 280, 25, "Address:");
    state.year     = new Fl_Int_Input(120, 70, 60, 25, "Date (Y/M/D):");
    state.month    = new Fl_Int_Input(190, 70, 40, 25);
    state.day      = new Fl_Int_Input(240, 70, 40, 25);
    state.filename = new Fl_Input(120, 100, 280, 25, "Filename:");

    state.product = new Fl_Input(120, 140, 150, 25, "Product:");
    state.price   = new Fl_Input(280, 140, 60, 25, "Price:");
    state.count   = new Fl_Int_Input(120, 170, 60, 25, "Count:");
    Fl_Button* add_btn = new Fl_Button(200, 170, 100, 25, "Add Purchase");
    add_btn->callback(add_purchase_cb, &state);

    state.purchase_list = new Fl_Browser(20, 210, 380, 120, "Pending Purchases:");

    Fl_Button* save_btn = new Fl_Button(150, 350, 120, 30, "Save Order");
    save_btn->callback(save_order_cb, &state);

    win.end();
    win.show();
    return Fl::run();
}
