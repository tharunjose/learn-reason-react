let now = MomentRe.momentNow();

type trip =
  | OneWay
  | OneWayWithDeparture(Js.Date.t)
  | RoundTrip
  | RoundTripWithOnlyDeparture(Js.Date.t)
  | RoundTripWithOnlyReturn(Js.Date.t)
  | RoundTripWithBothDates(Js.Date.t, Js.Date.t);

let tripToString = x =>
  switch (x) {
  | OneWay
  | OneWayWithDeparture(_) => "OneWay"
  | RoundTrip
  | RoundTripWithOnlyDeparture(_)
  | RoundTripWithOnlyReturn(_)
  | RoundTripWithBothDates(_, _) => "RoundTrip"
  };

let isOneWay = x =>
  switch (x) {
  | OneWay
  | OneWayWithDeparture(_) => true
  | RoundTrip
  | RoundTripWithOnlyDeparture(_)
  | RoundTripWithOnlyReturn(_)
  | RoundTripWithBothDates(_, _) => false
  };

let debug =
  fun
  | OneWay => "One Way"
  | OneWayWithDeparture(departure) =>
    "One Way: " ++ Js.Date.toDateString(departure)
  | RoundTrip => "Round Trip"
  | RoundTripWithOnlyDeparture(departure) =>
    "Round Trip (departure): " ++ Js.Date.toDateString(departure)
  | RoundTripWithOnlyReturn(return) =>
    "Round Trip (return)" ++ Js.Date.toDateString(return)
  | RoundTripWithBothDates(departure, return) =>
    "Round Trip (departure): "
    ++ Js.Date.toDateString(departure)
    ++ " (return): "
    ++ Js.Date.toDateString(return);

let debugStateChange = (currentState, nextState) => {
  Js.log("###");
  Js.log("Current State :-");
  Js.log(debug(currentState));
  Js.log(debug(nextState));
  Js.log("###");
};

let s = React.string;
let formTargetValue = event => ReactEvent.Form.target(event)##value;
let isEmptyDateString = x => x |> Js.String.trim |> Js.String.length == 0;
let formattedDateValue = x =>
  x |> Js.Date.toISOString |> Js.String.slice(~from=0, ~to_=10);

let today = MomentRe.momentNow();

//Validations
let isDateTodayOrAfter = x => {
  MomentRe.Moment.isSameWithGranularity(x, today, `day)
  || MomentRe.Moment.isSameOrAfter(x, today);
};

[@react.component]
let make = (~name) => {
  let (state, setState) = React.useState(() => OneWay);

  // Handle functions

  let handleDepartureDateChange = dateInput => {
    Js.log(dateInput);
    let nextState = () =>
      switch (state) {
      | OneWay => OneWayWithDeparture(Js.Date.fromString(dateInput))
      | OneWayWithDeparture(_) =>
        isEmptyDateString(dateInput)
          ? OneWay : OneWayWithDeparture(Js.Date.fromString(dateInput))
      | RoundTripWithOnlyDeparture(_) =>
        isEmptyDateString(dateInput)
          ? RoundTrip
          : RoundTripWithOnlyDeparture(Js.Date.fromString(dateInput))
      | RoundTripWithBothDates(_, return) =>
        isEmptyDateString(dateInput)
          ? RoundTripWithOnlyReturn(return)
          : RoundTripWithBothDates(Js.Date.fromString(dateInput), return)
      | RoundTrip =>
        RoundTripWithOnlyDeparture(Js.Date.fromString(dateInput))
      | RoundTripWithOnlyReturn(return) =>
        RoundTripWithBothDates(Js.Date.fromString(dateInput), return)
      };
    if (isEmptyDateString(dateInput)
        || isDateTodayOrAfter(dateInput |> MomentRe.moment)) {
      debugStateChange(state, nextState());
      setState(_ => nextState());
    };
  };

  let handleReturnDateChange = dateInput => {
    let nextState =
      switch (state) {
      | OneWay
      | OneWayWithDeparture(_) => OneWay
      | RoundTrip =>
        isEmptyDateString(dateInput)
          ? RoundTrip
          : RoundTripWithOnlyReturn(Js.Date.fromString(dateInput))
      | RoundTripWithOnlyDeparture(departure) =>
        isEmptyDateString(dateInput)
          ? state
          : RoundTripWithBothDates(departure, Js.Date.fromString(dateInput))
      | RoundTripWithOnlyReturn(_) =>
        isEmptyDateString(dateInput)
          ? state : RoundTripWithOnlyReturn(Js.Date.fromString(dateInput))
      | RoundTripWithBothDates(departure, _) =>
        isEmptyDateString(dateInput)
          ? state
          : RoundTripWithBothDates(departure, Js.Date.fromString(dateInput))
      };
    debugStateChange(state, nextState);
    setState(_ => nextState);
  };

  let handleTripChange = x =>
    switch (x) {
    | OneWay => RoundTrip
    | OneWayWithDeparture(departure) => RoundTripWithOnlyDeparture(departure)
    | RoundTrip => OneWay
    | RoundTripWithOnlyDeparture(departure) => OneWayWithDeparture(departure)
    | RoundTripWithOnlyReturn(_) => OneWay
    | RoundTripWithBothDates(departure, _) => OneWayWithDeparture(departure)
    };

  let handleChange = _ => {
    let nextState = handleTripChange(state);
    debugStateChange(state, nextState);
    setState(_ => nextState);
  };

  // Values
  let departureDateValue = x =>
    switch (x) {
    | OneWay
    | RoundTrip
    | RoundTripWithOnlyReturn(_) => ""
    | OneWayWithDeparture(departure)
    | RoundTripWithOnlyDeparture(departure)
    | RoundTripWithBothDates(departure, _) => departure |> formattedDateValue
    };

  let returnDateValue = x =>
    switch (x) {
    | OneWay
    | RoundTrip
    | OneWayWithDeparture(_)
    | RoundTripWithOnlyDeparture(_) => ""
    | RoundTripWithOnlyReturn(return)
    | RoundTripWithBothDates(_, return) => return |> formattedDateValue
    };

  let dateInput = (~label, ~handleDateChange, ~dateValue) =>
    <label>
      <p className="mb-2 text-center"> {s(label)} </p>
      <input
        className="border-2 border-gray-500"
        type_="date"
        value=dateValue
        onChange={event => event |> formTargetValue |> handleDateChange}
      />
    </label>;

  let departOn =
    <div className="mr-8">
      {dateInput(
         ~label="Depart On",
         ~handleDateChange=handleDepartureDateChange,
         ~dateValue=departureDateValue(state),
       )}
    </div>;

  let returnOn =
    dateInput(
      ~label="Return On",
      ~handleDateChange=handleReturnDateChange,
      ~dateValue=returnDateValue(state),
    );

  let datePicker =
    switch (state) {
    | OneWay
    | OneWayWithDeparture(_) => departOn
    | RoundTrip
    | RoundTripWithOnlyDeparture(_)
    | RoundTripWithOnlyReturn(_)
    | RoundTripWithBothDates(_, _) => <> departOn returnOn </>
    };

  <div className="bg-gray-200 h-screen">
    <h1 className="text-5xl text-center"> {React.string("Booking")} </h1>
    <div className="p-8 flex items-center justify-center">
      <label className="mr-8">
        <input
          onChange=handleChange
          name
          value="OneWay"
          checked={isOneWay(state)}
          className="mr-2"
          type_="radio"
        />
        <span> {s("One Way")} </span>
      </label>
      <label>
        <input
          onChange=handleChange
          name
          className="mr-2"
          value="RoundTrip"
          checked={!isOneWay(state)}
          type_="radio"
        />
        <span> {s("Round Trip")} </span>
      </label>
    </div>
    <div className="flex justify-center"> datePicker </div>
  </div>;
};