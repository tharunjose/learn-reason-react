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
  | OneWayWithDeparture(_) => "One Way"
  | RoundTrip
  | RoundTripWithOnlyDeparture(_)
  | RoundTripWithOnlyReturn(_)
  | RoundTripWithBothDates(_, _) => "Round Trip"
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