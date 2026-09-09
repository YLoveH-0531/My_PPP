/**
 * @file exer_21_12.cpp
 * @author KaKaRot
 * @brief
 * @version 0.1
 * @date 2026-09-08
 *
 * @copyright Copyright (c) 2026
 */

#include "Order.h"
#include "OrderQuery.h"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include <sstream>
#include <string>
#include <vector>

#ifndef DATA_DIR
#error "DATA_DIR must be defined by CMake. Use cmake to build this project."
#endif

const std::string DATA = DATA_DIR;

namespace {

struct OrderQueryState {
    Fl_Input*       filename;
    Fl_Input*       customer_query;
    Fl_Text_Buffer* buffer;
    std::vector<Order> orders;
};

void load_cb(Fl_Widget*, void* v)
{
    auto* s = static_cast<OrderQueryState*>(v);
    s->orders.clear();
    read_orders(DATA + std::string(s->filename->value()), s->orders);
    std::string msg = "Loaded " + std::to_string(s->orders.size()) + " orders.";
    s->buffer->text(msg.c_str());
}

void find_customer_cb(Fl_Widget*, void* v)
{
    auto* s = static_cast<OrderQueryState*>(v);
    auto matches = find_by_customer(s->orders, s->customer_query->value());
    std::ostringstream oss;
    if (matches.empty()) oss << "No orders found.\n";
    for (const auto& o : matches) print(oss, o);
    s->buffer->text(oss.str().c_str());
}

void total_value_cb(Fl_Widget*, void* v)
{
    auto* s = static_cast<OrderQueryState*>(v);
    std::string msg = "Total value: " + std::to_string(total_value(s->orders));
    s->buffer->text(msg.c_str());
}

void list_all_cb(Fl_Widget*, void* v)
{
    auto* s = static_cast<OrderQueryState*>(v);
    std::ostringstream oss;
    for (const auto& o : s->orders) print(oss, o);
    s->buffer->text(oss.str().c_str());
}

}  // namespace

int main()
{
    OrderQueryState state{};
    Fl_Window win(500, 420, "Order Query");

    state.filename = new Fl_Input(120, 10, 280, 25, "Filename:");
    state.filename->value("exer_21_9_orders_a.txt");
    Fl_Button* load_btn = new Fl_Button(410, 10, 70, 25, "Load");
    load_btn->callback(load_cb, &state);

    state.customer_query = new Fl_Input(120, 45, 280, 25, "Customer:");
    Fl_Button* find_btn = new Fl_Button(410, 45, 70, 25, "Find");
    find_btn->callback(find_customer_cb, &state);

    Fl_Button* total_btn = new Fl_Button(20, 80, 150, 25, "Total Value");
    total_btn->callback(total_value_cb, &state);
    Fl_Button* list_btn = new Fl_Button(180, 80, 150, 25, "List All");
    list_btn->callback(list_all_cb, &state);

    state.buffer = new Fl_Text_Buffer();
    auto* results = new Fl_Text_Display(20, 115, 460, 280, "Results:");
    results->buffer(state.buffer);

    win.end();
    win.show();
    return Fl::run();
}
