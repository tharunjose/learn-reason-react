type trip_ =
  | OneWay
  | OneWayWithDeparture(Js.Date.t)
  | RoundTrip
  | RoundTripWithOnlyDeparture(Js.Date.t)
  | RoundTripWithOnlyReturn(Js.Date.t)
  | RoundTripWithBothDates(Js.Date.t, Js.Date.t);

let tripToString_ =
  fun
  | OneWay
  | OneWayWithDeparture(_) => "One Way"
  | RoundTrip
  | RoundTripWithOnlyDeparture(_)
  | RoundTripWithOnlyReturn(_)
  | RoundTripWithBothDates(_, _) => "Round Trip";

type trip =
  | OneWay
  | RoundTrip;

let tripToString =
  fun
  | OneWay => "OneWay"
  | RoundTrip => "RoundTrip";

let formTargetValue = event => ReactEvent.Form.target(event)##value;

[@react.component]
let make = (~name) => {
  let (state, setState) = React.useState(() => OneWay);
  let handleChange = (state, _) => {
    setState(_ => state);
  };

  let dateInput = (~label, ~handleDateChange) =>
    <label>
      <p className="mb-2 text-center"> {React.string(label)} </p>
      <input
        className="border-2 border-gray-500"
        type_="date"
        onChange={event =>
          event |> formTargetValue |> Js.Date.fromString |> handleDateChange
        }
      />
    </label>;

  let handleDepartureDateChange = date =>
    Js.log("This is departure date: " ++ Js.Date.toISOString(date));

  let handleReturnDateChange = date =>
    Js.log("This is return date: " ++ Js.Date.toISOString(date));

  let departOn =
    <div className="mr-8">
      {dateInput(
         ~label="Depart On",
         ~handleDateChange=handleDepartureDateChange,
       )}
    </div>;

  let returnOn = {
    dateInput(~label="Return On", ~handleDateChange=handleReturnDateChange);
  };

  let datePicker =
    switch (state) {
    | OneWay => departOn
    | RoundTrip => <> departOn returnOn </>
    };

  <div className="bg-blue-200 h-screen">
    <h1 className="text-5xl text-center"> {React.string("Booking")} </h1>
    <div className="p-8 flex items-center justify-center">
      <label className="mr-8">
        <input
          onChange={handleChange(OneWay)}
          name
          value={tripToString(OneWay)}
          checked={OneWay == state}
          className="mr-2"
          type_="radio"
        />
        <span> {React.string("One Way")} </span>
      </label>
      <label className="mr-8">
        <input
          onChange={handleChange(RoundTrip)}
          name
          value={tripToString(RoundTrip)}
          checked={RoundTrip == state}
          className="mr-2"
          type_="radio"
        />
        <span> {React.string("Round Trip")} </span>
      </label>
    </div>
    <div className="flex justify-center pb-8"> datePicker </div>
  </div>;
};