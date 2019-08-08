let toCelcius = x => (x -. 32.) *. 5. /. 9.;

let toFarenheit = x => x *. 9. /. 5. +. 32.;

let formTargetValue = event => ReactEvent.Form.target(event)##value;

[@react.component]
let make = () => {
  let (celcius, setCelcius) = React.useState(() => 31.);

  <div className="bg-blue-200 ">
    <h1 className="text-1 text-center"> {React.string("Temperature")} </h1>
    <div className="p-8 flex items-center justify-center">
      <input
        type_="text"
        value={celcius |> Js.Float.toFixed}
        onChange={event => {
          let celcius = event |> formTargetValue |> Js.Float.fromString;
          setCelcius(_ => celcius);
        }}
        className="w-20 px-4 py-1 border border-gray-400 focus:border-blue-400 mr-2"
        placeholder={j|°C|j}
      />
      <span className="mr-2 uppercase"> {React.string("to")} </span>
      <input
        type_="text"
        value={celcius |> toFarenheit |> Js.Float.toString}
        onChange={event => {
          let celcius =
            event |> formTargetValue |> Js.Float.fromString |> toCelcius;
          setCelcius(_ => celcius);
        }}
        className="w-20 px-4 py-1 border border-gray-400 focus:border-blue-400 mr-2"
        placeholder={j|°F|j}
      />
    </div>
  </div>;
};