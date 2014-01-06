# How to create a Jenkins plugin
Based from [this link][jenkins_plugin]


## Introduction
Jenkins provides extensibility points, which are interfaces that model an aspect
of a build system. Jenkins allows plugins to contribute to the implementation of
those interfaces. [This document][extensibility_points] describes all the
extension points available in Jenkins.

One of the interface that we will play is a _Builder_. Builders build a project
(e.g Ant, Maven, etc)

## Setup
You need to modify your `~/.m2/settings.xml` for development so that IDEs such
as Intellij will know where to find the artifcacts. See [this
doc][jenkins_plugin] to know what to add to your `settings.xml`.


## Scaffolding with Maven
After performing the setup, you can generate a new jenkins plugin scaffold by
using this command:

```
mvn -U org.jenkins-ci.tools:maven-hpi-plugin:create
```

The `perform` method from the abstract class will perform the 'actual' work.

[jenkins_plugin]: https://wiki.jenkins-ci.org/display/JENKINS/Plugin+tutorial
[extensibility_points]: https://wiki.jenkins-ci.org/display/JENKINS/Extension+points#Extensionpoints-hudson.slaves.Cloud


# Writing a Hudson Plugin
Based on [this link][first_hudson_plugin]

## Maven scaffolding
Run this command to generate a basic Hudson plugin:
```
mvn hpi:create
```
`src/main/java`: Java source files
`src/main/resources`: Jelly view files of the plugin
`src/main/webapp`: Static resources of the plugin, such as images and HTML files.

## Building
```
mvn package
```

To run the project and see the result of the extension added by this skeleton
hudson plugin, run:
```
mvn hpi:run
```

## Extensions
The sample plugin will add an extension to the Hudson builder interface.
`HelloBuilder` simply prints out the text "Hello <name>" in the build console
log. It also provides UI to configure the `HelloBuilder` extension.

Hudson provides the concept of Extension points and Extensions to facilitate
contribution of functionalities to the core platform by plugins.

Extension points are interfaces that encapsulate entry points to extend certain
services or functionality of a service provided by the core platform.

A job, which is a buildable project consists of several configurable area and
build steps. Some of the build steps are:

- SCM checkout
- Pre-build: Indicate build is starting
- Build wrapper: prepare an environment for the build
- Build runs: invoke maven, ant etc
- Recording: Record the output from the build
- Notification: send out notifications

Builders are responsible for building jobs. Extension point provided by Hudson
to contribute to this builder is called `Builder`.

## Examining the extension
In order for Hudson to understand a class as an extension, it must:

- extend a class that advertise itself as an extension point
- implement the required abstract methods to extend the functionality
- tell Hudson that the particular class is an extension

The class `HelloWorldBuilder` extends the class `Builder` which is an extension
point for the Builder interface.

Abstract method that is needed to be implemented by the Builder extension is:
```java
public boolean perform
```

Finally, to tell Hudson that the class is an extension to some extension point,
it must be annotated with the annotation `@Extension`.

The annotation `@Extension` at the inner class `DescriptorImpl` tells Hudson the
class is an extension.

Side note: It seems that the `@Extension` annotation is always used with the
inner class that extends a Descriptor class.

## perform
The `BuildStep.perform` gives access to 3 objects:

- Build: Object representing the build of the job being performed. You have
  access to:
  * Project
  * Workspace: folder where the build happens
  * result: result of the build until this step
- Launcher: used to launch the build of this job
- BuildListener: interface to communicate status of build steps being performed
  in this builder and send any console message from this Build Step to Hudson

e.g
```java
listener.getLogger().println("Bonjour, "+name+"!");
```

With the Launcher object, it is easy to:
- Use the launcher to execute an external executable
- Send the result of execution to the console

```java
List<Cause> buildStepCause = new ArrayList();
buildStepCause.add(new Cause() {
        public String getShortDescription() {
            return "Build Ste started by Hello Builder";
        }
        });
listener.started(buildStepCause);

ArgumentListBuilder args = new ArgumentListBuilder();
if (launcher.isUnix()) {
    args.add("/bin/ls");
    args.add("-la");
} else {
    args.add("dir");
}

args.add(System.getProperty("user.home"));

try {
    int r;
    r = launcher.launch().cmds(args).stdout(listener).join();

    if (r != 0) {
        listener.finished(Result.FAILURE);
    } else {
        listener.finished(Result.SUCCESS);
    }
}
```

When a build step is started or stopped, you need to tell Hudson about it. This
is done via:
```java
listener.started(buildStepCause);
listener.finished(Result.SUCCESS);
```

This is important because:

- Hudson can then show the progress of the overall build of the job
- When a build step fails, Hudson must stop the overall progress of the build
  and mark the build as FAILED.

Use the Launcher interface to launch your external executable.

The launcher will correct launch the application in the master or slave node the
job is running. The standard output of the launcher is hooked to the listener.

## Extending your extension
There are 2 ways to configure your extension. One is local to the area of the
functionality the plugin extends and the other via the Hudson wide Global
configuration.

Hudson uses Jelly for UI. Jelly converts XML based jelly definitions to HTML,
Javascript and Ajax. Hudson provides a number of Jelly tags for your
convenience.

The model objects are bound to these tag attributes via an expression language
called `Jexi`. The rendered code includes information from the model objects to
which their attributes are bound to.

The jelly files to render the UI has the extension `.jelly`. They reside in the
resources directory of the plugin. The folder under which the jelly files must
reside **should** have a path hierarchy similar to the package name of the model
class, plus the name of the model class itself.

So the configuration file must reside under the folder
`org/sample/hudson/HelloWorldBuilder`.

Hudson also uses another convention to tell if the configuration file is meant
for local or global configuration. If the config is named `config.jelly` is used
as a local configuration file and its content is included in the configuration
of the functionality that this extension extends.

So the `config.jelly` of HelloWorldBuilder will be included in the Job
Configuration page to configure the HelloWorldBuilder extension.

There are two main tags playing the role of user interaction:

- entry: tell hudson the enclosing tags are considered as user interaction
  elements and submitted via HTML form

- textbox: renders simple HTML text field whose value will be sent back to the
  server.

```xml
<f:entry title="Name" field="Name">
    <f:textbox />
</f:entry>
```

## Understanding the UI rendering
To be able to get the 'Question' icon next to the text field, Hudson uses the
convention that if a file named 'help-name.html' is present, it will display the
'Question' icon.

`help-{fieldName}.html`

Notice the `field="Name"` in the xml listing above.

If you want to use information from Hudson model objects in your help text, you
should have jelly content in the field Help file and the extension of the
file-name should be `.jelly` instead of `.html`.

e.g `help-name.jelly`
```xml
<j:jelly xmlns:j="jelly:core" xmlns:st="jelly:stapler" xmlns:d="jelly:define">
    <div>
        Welcome to ${app.displayName}.
        Enter your name in the Field.
    </div>
</j:jelly>
```

## Interaction between UI and model
`HelloBuilder` is a Hudson model object. It encapsulates data. UI can interact
with this model to get and display its data or get information from user via
fields in the UI and update the model data.

When the server side of the Hudson application received the request for job
configuration page, it included the HelloBuilder configuration snippet in the
job configuration page. The Jelly renderer is responsible for substituting the
corresponding value for the Jexl expression after evaluating it.

By default, Hudson registers 3 identifiers for the model objects to the Jexl
expression evaluator:

- app: the hudson application itself. e.g `${app.displayName}` evaluates to
  `Hudson.getDisplayName()`.
- it: model object to which the Jelly UI belongs to. `${id.name}` evaluates to
  `HelloWorldBuilder.getName()`.
- h: A global utility function which provides static utility methods. e.g
  `${h.clientLocale}` evaluate to `Functions.getClientLocale()`.

When the UI is submitted, Hudson re-creates the model by passing the
corresponding value via the constructor. Therefore the constructor of the model
must have a parameter whose name matches the name of the field.

e.g
```xml
<f:entry title="Name" field="name">
```
So the constructor of your HelloBuilder must have a parameter with the name
"name".

```java
@DataBoundConstructor
public HelloWorldBuilder(String name) {
    this.name = name;
}
```
The annotation `@DataBoundConstructor` hints to Hudson that this extension is
bound to a field and on UI submission, it must be reconstructed using the value
of the fields submitted.

Also, it must have a getter with the name of the field for the `config.xml` to
get the data for the second time around when the project is configured again.

```java
public String getName() {
    return name;
}
```

The information is persisted along with the project configuration.

## UI validation methodology
For UI validation, ubon submission, Hudson finds the extension, then executes
the method `doCheck + {nameOfTheField}` as part of the Ajax URL.

If the extension class does not include the `doCheck` methods, then Hudson won't
do any validation checks.

```java
public FormValidation doCheckName(@QueryParameter String value)
    throws IOException, ServletException {
        if (value.length() == 0) {
            return FormValidation.error("Please set a name");
        }

        if (value.length() < 4) {
            return FormValidation.warning("Isn't that too short?");
        }
        return FormValidation.ok();
}
```

The warning method will return a message in brownish yellow warning icon.

## Adding your Extension's global configuration
Some of the configuration extension could be global. That will go into the
Hudson's Global configuration page.


[first_hudson_plugin]: http://wiki.eclipse.org/Hudson-ci/writing-first-hudson-plugin
