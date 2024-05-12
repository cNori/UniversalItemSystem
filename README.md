> [!IMPORTANT]
> <br>System in under development is missing some key features</br>
> 1. Multi Item Data Container
> 2. Visual Multi Item Data Container
> 3. NetCode for picking up items and dropping it
> 4. needs a pr https://github.com/FlaxEngine/FlaxEngine/pull/2591 with was not yet merged to engine

# **Universal Item System for Flax Engine**

Allows to make any kind of item for any kind of game

> [!NOTE]
> System has some custom net code.

# Design Inspiraction
Pulled a idea from Minecraft item system.<br>Minecraft has very interesting item system just by looking at attributes from user view</br> So this system follows this idea

# Item Structure

| Type              | Role                                    |
| -------------     | -------------                           |
| Item              | Single Item Data Container              |
| Item::Data        | Runtime Item representation             |
| Item::Attribute   | Runtime item attribute representation   |
| Item::Asset       | Static item representation              |

> [!IMPORTANT]
> Attributes can be any type
> <br>The internal data is stored as byte array max size is 127 bytes</br>
> Going past the 127 bytes limit will cause the bytes to chopp by the item system

# Attribute types
- null - no data
- Bool - uses 1 byte
- Decimal - it can be a float or double depending on value uses 4 to 8 byte
- Text - a string with max size of 63 w_chars
- UnsindedInteger - any unsinded integer uses 1 to 8 byte
- Integer - any integer uses 1 to 8 byte
- Custom - a wild card data can be anything
> [!CAUTION]
> Custom is unsafe because it is a wild card when using it make sure the name is unique and data size is known and don't trust the network replicated values <br>try to not push large blocks of data to attributes</br>
