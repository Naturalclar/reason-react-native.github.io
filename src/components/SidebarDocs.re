open Belt;
open ReactNative;
open ReactMultiversal;

let styles =
  StyleSheet.create(
    Style.{
      "container": viewStyle(~height=100.->pct, ~overflow=`scroll, ()),
      "title": textStyle(~fontSize=16., ~fontWeight=`_600, ~color="#333", ()),
      "link": viewStyle(),
      "linkActive":
        viewStyle(
          ~shadowColor=Predefined.Colors.blue,
          ~shadowOffset=offset(~width=-3., ~height=0.),
          ~shadowOpacity=1.,
          ~shadowRadius=0.,
          (),
        ),
      "linkText":
        textStyle(~fontSize=14., ~fontWeight=`_400, ~color="#555", ()),
      "linkTextActive": textStyle(~color=Predefined.Colors.blue, ()),
      "notlinkText": textStyle(~color="#ddd", ()),
    },
  );

type item = (string, string);

let makeSections =
    (sections: list(Consts.section))
    : array(VirtualizedSectionList.section(item, unit)) => {
  sections
  ->List.toArray
  ->Array.map(section =>
      {
        "key": Some(section.title),
        "data": section.data->List.toArray,
        "renderItem": None,
        "ItemSeparatorComponent": None,
        "keyExtractor": None,
        "sectionData": None,
      }
    );
};

let sections = makeSections(Consts.sections);

[@react.component]
let make = (~docsIndex, ~currentLocation) => {
  <div
    className="stick"
    style={ReactDOMRe.Style.make(
      ~top="0",
      ~display="flex",
      ~flexDirection="column",
      ~overflowY="auto",
      ~height="100vh",
      (),
    )}>
    <Spacer />
    /* until we have client side react too, section list won't sticky headers...
       <div className="stick" style={ReactDOMRe.Style.make(~height="100vh", ())}>
          <SpacedView vertical=SpacedView.S style=Style.(style(~flex=1., ()))>
            <SectionList
              initialNumToRender=200 // SSR
              // disableVirtualization=true // SSR
              style=Style.(style(~flex=1., ()))
              contentContainerStyle=Style.(style(~flex=1., ()))
              sections
              renderItem={renderItemBag => {
                let (title, link) = renderItemBag##item;
                <TextLink key=title href={Consts.baseUrl++ "/en/" ++ link} style=styles##link>
                  title->React.string
                </TextLink>;
              }}
              renderSectionHeader={renderSectionBag =>
                <SpacedView
                  vertical=SpacedView.S
                  horizontal=SpacedView.None
                  style=styles##titleContainer>
                  <Text style=styles##title>
                    {renderSectionBag##section##key
                     ->Option.getWithDefault("")
                     ->String.uppercase
                     ->React.string}
                  </Text>
                </SpacedView>
              }
              keyExtractor={((title, link), _index) => title ++ link}
            />
          </SpacedView>
          */
    {Consts.sections
     ->List.map(section =>
         <SpacedView key={section.title} vertical=SpacedView.None>
           <div
             className="stick"
             style={ReactDOMRe.Style.make(
               ~top="0",
               ~display="flex",
               ~flexDirection="column",
               ~background=
                 "linear-gradient("
                 ++ Consts.Colors.light
                 ++ " 75%, "
                 ++ Consts.Colors.light0
                 ++ ")",
               ~zIndex="1",
               (),
             )}>
             <SpacedView vertical=M horizontal=None>
               <Text style=styles##title>
                 {section.title->String.uppercase_ascii->React.string}
               </Text>
             </SpacedView>
           </div>
           {{
              section.data
              ->List.map(((title, link)) => {
                  let fulllink = "en/" ++ link;
                  let isActive =
                    currentLocation##pathname
                    |> Js.String.startsWith(fulllink ++ "index.html");
                  let isApi = link |> Js.String.startsWith("docs/apis/");
                  let isComponent =
                    link |> Js.String.startsWith("docs/components/");
                  !isApi
                  && !isComponent
                  || (isApi || isComponent)
                  && docsIndex->Array.some(path =>
                       link == "docs/" ++ path ++ "/"
                     )
                    ? <ViewLink
                        key=title
                        href={Consts.baseUrl ++ "/" ++ fulllink}
                        style={Style.arrayOption([|
                          Some(styles##link),
                          isActive ? Some(styles##linkActive) : None,
                        |])}>
                        <SpacedView horizontal=XS vertical=XXS>
                          <Text
                            style={Style.arrayOption([|
                              Some(styles##linkText),
                              isActive ? Some(styles##linkTextActive) : None,
                            |])}>
                            title->React.string
                          </Text>
                        </SpacedView>
                      </ViewLink>
                    : <SpacedView horizontal=XS vertical=XXS>
                        <Text
                          key=title
                          style={Style.array([|
                            styles##link,
                            styles##notlinkText,
                          |])}>
                          title->React.string
                        </Text>
                      </SpacedView>;
                });
            }
            ->List.toArray
            ->React.array}
           <Spacer size=Spacer.S />
         </SpacedView>
       )
     ->List.toArray
     ->React.array}
    <Spacer />
  </div>;
};
