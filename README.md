<p align="center"><img src="https://wallpapercave.com/wp/wp9357389.jpg"></p>

<h1 align="center">Autopilot Plus </h1>

<h3 align="center">
 System to perform fully autonomous flights, with the extra ability to intercept civilian aircrafts
</h3>

<p align="center">
<!--<a href="https://github.com/r-spacex/SpaceX-API/actions?query=workflow%3ATest"><img src="https://img.shields.io/github/workflow/status/r-spacex/SpaceX-API/Test?style=flat-square"></a>-->
<a href="https://github.com/r-spacex/SpaceX-API/releases"><img src="https://img.shields.io/github/release/ngomezcn/AutopilotPlus.svg?longCache=true&style=flat-square"></a>
<a href="https://en.wikipedia.org/wiki/Representational_state_transfer"><img src="https://img.shields.io/badge/interface-REST-brightgreen.svg?longCache=true&style=flat-square"></a>
</p>

<h4 align="center">
  <i>
    The software is being tested in X-Plane12, which is FAA-certified flight simulation software.
  </i>
</h4>

<h3 align="center">
    <a href="docs/README.md">Docs</a> - 
    <a href="docs/apidoc">API</a> - 
    <a href="docs/how-to-deploy">Deploy</a> -
    <a href="https://www.x-plane.com/pro/certified/">FAA X-Plane</a> 
<br/>
</h3>

## Software architecture
<h3>Avionic Control System</h3>  
<p align="left"><img width="30%" src="./resources/images/acs.svg"> </p>

<h3>Components overview</h3>  
<p align="left"><img width="100%" src="./resources/images/construct_components_overview.svg"></p>

<h3>ACS Detail</h3>  
<p align="left"><img width="70%" src="./resources/images/acs_det.svg"></p>

## Intercepction of airpline by turning radius


Diagram showing two possible paths of interception by turning radius. 
At this diagram pursuer and target have constant speeds <br>
Pursuer (P) | Target (T) 
<p align="left"><img width="60%" src="./resources/images/interceptance_figure.png"></p>

- Limitations:
  - The pursuer cannot reverse its direction to intercept a target. 
  - The pursuer cannot intercept a target inside its turning-circle.
  - The pursuer may perform at most one complete turn
  - pursuer speed is greater than the target???s speed (may be is not absolutely necesary)
