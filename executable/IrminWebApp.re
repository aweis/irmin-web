module Mem_store = Irmin_mem.KV(Irmin.Contents.String);

let config = Irmin_mem.config();

let repo = Mem_store.Repo.v(config);

open Lwt.Infix;
let master = config => Mem_store.Repo.v(config) >>= Mem_store.master;

let info = msg => {
  let date = Int64.zero;
  let author = "Adam Weis";
  Irmin.Info.v(~date, ~author, msg);
};

let infof = fmt => Fmt.kstrf((str, ()) => info(str), fmt);

Js_of_ocaml.Js.export(
  "Irmin",
  [%js
    {
      as _;
      pub add = (x, y) => x +. y;
      pub abs = x => abs_float(x);
      val foo = 8;
      val zero = 0.
    }
  ],
);

let pp_commit_info = (info: Irmin.Info.t) =>
  Irmin.Info.author(info)
  ++ ": "
  ++ Irmin.Info.message(info)
  ++ ": "
  ++ Int64.to_string(Irmin.Info.date(info));

let main =
  Mem_store.Repo.v(config)
  >>= Mem_store.master
  >>= (
    t =>
      Mem_store.history(t)
      >|= (
        history_res =>
          Printf.printf(
            "0. is the history empty? %b\n",
            Mem_store.History.is_empty(history_res),
          )
      )
      >>= (
        () => {
          print_endline("setting [a,b,c]");
          Mem_store.set_exn(
            t,
            ["a", "b", "c"],
            "Message 1",
            ~info=infof("COMMIT 1"),
          );
        }
      )
      >>= (
        () =>
          Mem_store.get(t, ["a", "b", "c"])
          >|= (s => print_endline("data at [a,b,c] is " ++ s))
      )
      >>= (
        () =>
          Mem_store.history(t)
          >|= (
            history_res =>
              Printf.printf(
                "1. is the history empty? %b\n",
                Mem_store.History.is_empty(history_res),
              )
          )
      )
      >>= (
        () => {
          print_endline("setting [a,b,d]");
          Mem_store.set_exn(
            t,
            ["a", "b", "d"],
            "Message 2",
            ~info=infof("COMMIT 2"),
          );
        }
      )
      >>= (
        () =>
          Mem_store.get(t, ["a", "b", "c"])
          >|= (s => print_endline("data at [a,b,c] is " ++ s))
      )
      >>= (
        () =>
          Mem_store.get(t, ["a", "b", "d"])
          >|= (s => print_endline("data at [a,b,d] is " ++ s))
      )
      >>= (
        () =>
          Mem_store.history(t)
          >|= (
            history_res =>
              Printf.printf(
                "2. is the history empty? %b\n",
                Mem_store.History.is_empty(history_res),
              )
          )
      )
      >>= (
        () =>
          Mem_store.history(t)
          >|= (
            history_res =>
              Printf.printf(
                "3. number of vertices %d\n number of edges %d\n",
                Mem_store.History.nb_vertex(history_res),
                Mem_store.History.nb_edges(history_res),
              )
          )
      )
      >>= (
        () =>
          Mem_store.history(t)
          >|= (
            history_res =>
              Mem_store.History.iter_vertex(
                v =>
                  print_endline(
                    "vertex! "
                    ++ (v |> Mem_store.Commit.info |> pp_commit_info),
                  ),
                history_res,
              )
          )
      )
  );
