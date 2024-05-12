> [!IMPORTANT]
> <br>System in under develimponent is missing some key fetures</br>
> 1. Multy Item Data Contener
> 2. Visual Multy Item Data Contener
> 3. NetCode for picking up items and droping it

# **Universal Item System for Flax Engine**

Allows to make anny kind of item for any kind of game

> [!NOTE]
> System has some custom net code.

# Design Inspiraction
Pulled a idea from Minecraft item system.<br>Minecraft has very interesting item system just by looking at attributes from user view</br> So this system fallows this idea

# Item Structure

| Type              | Role                                    |
| -------------     | -------------                           |
| Item              | Single Item Data Contener               |
| Item::Data        | Runtime item representacion             |
| Item::Attribute   | Runtime item attribute representacion   |
| Item::Asset       | Static item representacion              |

> [!IMPORTANT]
> Attributes can be any type
> <br>The internal data is stored as byte array max size is 127 bytes</br>
> Going past the 127 bytes limit wil couse the bytes to chopp by the item system

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
