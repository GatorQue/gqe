The coding standards used throughout the GatorQue Engine (GQE) libraries and files are as follows:

# FAQ #

  * What is a Coding Standard? Coding Standards are rules that govern how the code should be written and maintained.

  * Why have a Coding Standard? Coding Standards help to promote a consistent style across multiple files and allow others to understand the style used.

  * Where did you come up with your Coding Standard? The Coding Standard presented below was developed by observations made in reviewing other professional code bases both at my work and online.

  * Can I change your Coding Standard? Coding Standards are meant to be updated over time as needs arise.  Be sure to provide a reasonable justification for changing the coding standards used by the project.

# C++ #
As a general rule, please try to maintain a familiar style when making changes to C++ files.  The following is a brief list of different C++ coding standard conventions that have proven useful to me and others when writing C++ code.  Feel free to suggest additional conventions to this list as needed.
## Naming Conventions ##
  * Class names shall be named using CamelCase naming conventions and will begin with a capital letter.  Exceptions to this include Template Classes and Interface Classes which will always start with prefix letter T and I to their Class names, respectively.
  * Class names will avoid acronyms whenever possible and be long enough to be descriptive, but short enough to not be too obnoxious.
  * Class names that are part of a group shall contain the group suffix as part of its name and be located within the group folder whenever possible.  Examples include the action, assets, loggers, states, and widget folders.
  * Filenames shall be named the same as the Class defined within them.  There should only be 1 class defined per file if possible.
  * Directories shall be created to group similar Classes together to make finding like classes easier whenever possible.  Exceptions to this include Interface classes and generally single instance classes like Manager classes (although this might change as more manager classes are added to the code base).
  * Class definitions should always be defined in the hpp file and the class methods should be defined in the cpp file.  Exceptions to this rule is Template classes which should have both definition and methods defined in the hpp file.  This prevents the dreaded circular #include dependency issues.  For example, the ILogger class references the App class and the App class uses the ILogger class to perform logging functions.  This means Template classes must be careful which classes they reference to prevent circular #include dependency issues.
  * Enumeration constants should be prefixed with a word related to the enumeration Name to more easily determine if the enumerations used are correct.
  * Local variables shall be prefixed with the letters "an" followed by the CamelCase name.  This makes local variables stand out and assists in making the code more human readable.
  * Method/parameter variables shall be prefixed with the letters "the" followed by the CamelCase name.  This makes method/parameter variables stand out and assists in the description of the variables in the method description block.
  * Class variables shall be prefixed with the letter "m" followed by the CamelCase name.  Exceptions to this rule include public variables in a class.  This makes member variables stand out and assists in making the code more human readable.
## Coding Conventions ##
  * All classes shall be forward declared in the **Library**`_`types.hpp file in the include/GQE/**Library** folder.  This makes it easier for other classes to reference a single file to obtain all forward declared classes within a **Library**.
  * All class headers shall be included explicitly in the **Library**.hpp file in the include/GQE folder.  This means that the user of the **Library** only needs to include the **Library**.hpp file to access all classes in the **Library**.
  * All code shall be written using Spaces only.
  * All code shall use 2 spaces for all indentations.  Code should be indented, not the curly braces when starting a new code block.
  * Curly braces shall be used on their own line whenever possible.  This makes is easier to comment out a "while" or "if" statement and still have the "while" or "if" block execute.
## Documentation Conventions ##
  * All C++ files shall be documented using the Doxygen standards.
  * All hpp and cpp files shall contain a History of changes at the beginning of the file to assist with tracing fixes.  At minimum this History of changes will contain a date and description of the changes.
  * All classes, methods, and examples shall be documented in the hpp files.
  * All parameters and return values shall be documented using Doxygen style conventions for each method in each class in the hpp file.
  * Cpp files shall only contain internal implementation comments as needed.