# Roadmap

Project Reunion is a set of libraries, frameworks, components, and tools that you can use in your apps to access powerful Windows platform functionality from all kinds of apps on many versions of Windows.

The roadmap for Project Reunion supports the primary goals of breaking down the barriers between Win32 and UWP and making the Windows developer platform more agile, compatible, modern and open.

For a general overview, see the [readme](https://github.com/microsoft/ProjectReunion/tree/master/docs) and [FAQ](https://github.com/microsoft/ProjectReunion/blob/master/docs/faq.md).

Here's how we intend to roll out Project Reunion *(all times/features/releases are tentative and subject to change)*:

<table>
  <tbody>
    <tr>
      <th>Reunion 0.5 (Q1 2021)</th>
      <th>Reunion 0.8 (Q2 2021)</th>
      <th>Reunion 1.0 (Q3 2021)</th>
      <th>2022 and Beyond</th>
    </tr>
    <tr>
    <td>
      <ul>
            <li>1st supported public release</li>
            <li>WinUI 3 works in <a href="https://docs.microsoft.com/en-us/windows/msix/desktop/desktop-to-uwp-packaging-dot-net">Desktop Packaged (Win32)</a> Apps</li>
            <li>Win2D</li>
            <li>DwriteCore</li>
            <li>MRTCore</li>
       </ul>
     </td>
     <td>
        <ul>
            <li>WinUI improvements</li>
            <li>Framework Package support for Unpackaged apps</li>
            <li>App Lifecycle API</li>
            <li>State Notifications</li>
         </ul>
     </td>
     <td>
        <ul>
            <li>Reunion Windowing support</li>
            <li>New Rendering and Input features</li>
            <li>Smooth WinUI input & composition system experience</li>  
         </ul>
     </td>
    <td>
        <ul>
            <li>Multiple releases per year</li>
            <li>Continue expanding existing app compatibility</li>
            <li>Continuous delivery of new easier to use capabilities</li>
         </ul>
     </td>
    </tr>
  </tbody>
</table>

## 2021 Focus Areas

The Windows platform team is currently focused on the four primary areas below for Project Reunion.

This isn't an exhaustive list: it's a sampling of some of the key infrastructure work we're doing to break down the barriers between Win32 and UWP and decouple the platform from the OS, plus some of the new functionality we're adding to enable new app capabilities and address top developer issues.

### 1. Coherent, modern interactions and UX design

* [WinUI 3](https://github.com/microsoft/microsoft-ui-xaml/blob/master/docs/roadmap.md) - the Windows 10 native UI platform for Win32 and UWP
* [WebView2](https://docs.microsoft.com/microsoft-edge/webview2/) - embedding web content in Windows apps using the new Edge (Chromium) engine
* [React Native Windows](https://github.com/microsoft/react-native-windows/projects/30) - now targeting WinUI
* [Modern Windowing](https://github.com/microsoft/ProjectReunion/discussions/370)

### 2. Optimized for the device hardware

* Touch, inking, display improvements
* ARM64 support
* Input 

### 3. Great system performance and battery life

* [Better options for app lifecycle management and power usage](https://github.com/microsoft/ProjectReunion/issues/111)
* [DirectWrite text rendering platform](https://github.com/microsoft/ProjectReunion/issues/112)
* [Local and Push Notifications](https://github.com/microsoft/ProjectReunion/discussions/371)
* Background Tasks

### 4. Hassle-free app discovery and management

* Enhanced app packaging
* Framework package deployment
* Auto upate for all app types

### 5. Platform unification and deployment

* Decoupling the Windows platform from the OS
  * faster updates that you can start using on day 1
* Ensuring features work on all supported Windows versions
  * initial min OS version = 1809
  * polyfilling features as needed
  * supporting both Win32 and UWP
  
* Moving engineering to GitHub
  * [Proposals](https://github.com/microsoft/ProjectReunion/issues?q=is%3Aissue+is%3Aopen+label%3A%22feature+proposal%22) -> [Specs](https://github.com/microsoft/ProjectReunion/tree/master/specs) -> [Code](https://github.com/microsoft/ProjectReunion/tree/master/dev)
