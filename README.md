# Book Store Management System

## Overview
This project is a sophisticated **Book Store Management System** built with C++20, designed to simulate the operations of a modern digital library and bookstore. It features a robust Command Line Interface (CLI) that orchestrates complex interactions between users (Administrators and Clients) and a diverse inventory of publications. The system emphasizes Object-Oriented Programming (OOP) principles, utilizing inheritance, polymorphism, encapsulation, and design patterns to create a scalable and maintainable architecture.

## Functionalities

### 1. User Roles & Interaction
The application supports two distinct user roles with specialized menus and capabilities:

#### Administrator
*   **Authentication:** Secure login with password protection.
*   **Inventory Management:**
    *   **Add Publications:** Create new Books, Manuals, Scientific Books, and Magazines.
    *   **Modify Status:** Activate, deactivate, or delete publications from the catalog.
    *   **Stock Control:** View and manage stock levels; clone existing publications to quickly add similar items.
    *   **Promotions:** Create and manage discount campaigns for specific items.
*   **User Management:**
    *   **View Clients:** Filter clients by username or email.
    *   **Statistics:** Analyze client activity, VIP status, and purchase history.
    *   **Logs:** Access system-wide logs tracking logins, account creations, and orders.
*   **Author Management:** Add new authors and view detailed profiles, including productivity scores and hierarchy rankings.
*   **Order Management:** View all orders, filter by status, and inspect specific order details.
*   **Statistics Dashboard:**
    *   View distribution of publication types.
    *   Analyze sales performance and revenue.
    *   Identify top-performing authors and products.

#### Client
*   **Account Management:**
    *   **Registration:** Create a new account with address and contact details.
    *   **Authentication:** Login using Email, Phone, or Username.
    *   **Wallet:** Deposit funds into a virtual wallet for purchases.
    *   **Loyalty Program:** Earn points per purchase and redeem them for discounts.
    *   **Profile:** View order history, manage delivery address, and track loyalty status (Standard, Loyal, VIP).
*   **Shopping Experience:**
    *   **Search Engine:** Advanced search by title, author, or type (Book, Manual, etc.).
    *   **Shopping Cart:** Add items (individual or bundles), view subtotal, and remove items.
    *   **Checkout:** Finalize orders using wallet balance and/or loyalty points.
    *   **Reviews:** Write rated reviews for purchased items; view reviews from other users.
*   **Second-Hand (Trade-In) Market:**
    *   **Trade-In:** "Sell" used books back to the store in exchange for store credit or discounts. The system evaluates the value based on condition and age.
    *   **Buy Used:** Purchase second-hand items at reduced prices.
    *   **Bundles:** Create custom "Second-Hand Bundles" from available used inventory for extra savings.
*   **Personalization:**
    *   **Suggestions:** Receive personalized book recommendations based on purchase history.

### 2. Inventory & Domain Logic
The system models a rich domain of sellable items with specific behaviors:

*   **Carte (Book):** The fundamental unit. Features dynamic pricing based on age and popularity (Bestseller logic).
*   **Manual (Textbook):** Specialized book with logic for educational validation (Ministry approved, outdated, etc.) and grade levels.
*   **Carte Stiintifica (Scientific Book):** Tracks academic impact factors, citations, and domain-specific value (e.g., IT vs. Medicine).
*   **Revista (Magazine):** Serial publication with frequency tracking and collectibility scores.
*   **Bundles (PachetSerie):** Grouping mechanism (Composite Pattern) to sell collections (e.g., "The Lord of the Rings Trilogy") as a single unit with aggregated pricing and discounts.

### 3. Business Logic Engines
*   **Dynamic Pricing:** Real-time price calculation considers:
    *   Base price & Publisher prestige.
    *   Physical condition (New vs. Used).
    *   Market demand (Popularity score).
    *   Active discounts.
*   **Trade-In Evaluator:** Algorithms to determine the buy-back price of a used book based on its physical state (New, Good, Worn) and market relevance.
*   **Recommendation Engine:** Analyzes user history to suggest new titles (e.g., "Since you bought 'Dune', you might like 'Foundation'").

## Architecture & Inheritance

The project creates a hierarchical structure to model the business domain effectively:

### Core Hierarchy
*   **`Publicatie` (Abstract Base):** Defines common attributes (title, price, stock) and the interface for all publishable items.
    *   **`Carte`:** Adds author, ISBN, and bestseller logic.
        *   **`Manual`:** Adds subject, grade, and approval status.
        *   **`CarteStiintifica`:** Adds research domain, academic level, and citation count.
    *   **`Revista`:** Adds issue number, frequency, and ISSN.

### Sales Hierarchy
To handle the sale of different physical instances (New vs. Used, Single vs. Bundle), a parallel hierarchy exists:
*   **`UnitateVanzare` (Abstract Base):** Represents any item that can be placed in a cart.
    *   **`CarteIndividuala`:** Wraps a `Carte` for sale.
    *   **`RevistaIndividuala`:** Wraps a `Revista` for sale.
    *   **`PachetSerie`:** A Composite implementation containing a list of `UnitateVanzare`.

### Key Classes
*   **`AppState`:** The central data repository (Singleton-like usage) holding lists of all entities (clients, books, orders).
*   **`BookStoreManager`:** The application controller handling the main loop, menu navigation, and user input.
*   **`Client`:** Encapsulates user data, wallet logic, and loyalty state.
*   **`Comanda`:** Represents a transaction, linking a Client to a list of `UnitateVanzare` items.
*   **`Autor`:** Represents content creators with logic for "Renown Score" and productivity.

## Data Persistence
The application features a custom serialization system to ensure data survives between sessions.
*   **Text-Based Storage:** Data is stored in human-readable `.txt` files in the `data/` directory (`autori.txt`, `clienti.txt`, `publicatii.txt`, etc.).
*   **Serialization:** Every class implements a `serializare()` method to convert its state into a delimited string.
*   **Deserialization:** Static factory methods (e.g., `dinString`) parse these strings to reconstruct objects, resolving relationships (like linking a Book to its Author by ID) during the load process.

## Exception Handling
A custom exception hierarchy rooted in `LibrarieException` ensures robust error management:
*   `DateInvalideException`: Handles bad user input.
*   `StocException`: Manages inventory errors (e.g., trying to buy out-of-stock items).
*   `ComandaExceptions`: Handles commerce logic errors (insufficient funds, empty cart).

## Design Patterns
*   **Factory Method:** Used in loading logic (`Publicatie::fabricare`) to instantiate the correct subclass based on data type.
*   **Composite:** `PachetSerie` allows bundles to be treated exactly like individual books.
*   **Strategy (Implicit):** Search and filter services use predicate-like logic to vary behavior.
